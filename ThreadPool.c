#include "ThreadPool.h"
#include "PointerList.h"
#include <Platform/PlatformCPU.h>
#include <threads.h>
#include <stdatomic.h>
#include <assert.h>
#include <Platform/Logger.h>

typedef struct
	{
	int ( *FunctionPointer ) ( void * );
	void *Argument;
	int TaskID;
	int Result;
	TaskStatus Status;
	} TaskData;

typedef struct ThreadPool
	{
	PointerList TaskQueue;
	int LastTaskID;
	mtx_t TaskQueueMutex;
	cnd_t WakeUpCondition;
	mtx_t WakeUpMutex;
	cnd_t TaskFinishedCondition;
	mtx_t TaskFinishedMutex;

	thrd_t *ThreadArray;
	atomic_int FreeThreads;
	unsigned ThreadCount;
	bool Quitting;
	} ThreadPool;

static unsigned ThreadPool_GetTaskCount ( ThreadPool *Pool )
	{
	unsigned Result = 0;
	mtx_lock ( &Pool->TaskQueueMutex );
	Result = PointerList_GetSize ( &Pool->TaskQueue );
	mtx_unlock ( &Pool->TaskQueueMutex );
	return Result;
	}

static int ThreadPool_ThreadWorker ( ThreadPool *Pool )
	{
	while ( Pool->Quitting == false )
		{
		mtx_lock ( &Pool->TaskQueueMutex );

		TaskData *CurrentTask = NULL;

		// Find the first queued task
		PointerListNode *TaskNode = NULL;
		for ( TaskNode = PointerList_GetFirst ( &Pool->TaskQueue ); TaskNode != NULL; TaskNode = PointerList_GetNextNode ( TaskNode ) )
			{
			TaskData *TempTask = ( TaskData * ) PointerList_GetNodeData ( TaskNode );
			if ( TempTask->Status == TASK_STATUS_QUEUED ) // Found a queued task
				{
				CurrentTask = TempTask;
				CurrentTask->Status = TASK_STATUS_RUNNING;
				break;
				}
			}

		mtx_unlock ( &Pool->TaskQueueMutex );

		if ( CurrentTask != NULL ) // There was a task. run it...
			{
			CurrentTask->Result = CurrentTask->FunctionPointer ( CurrentTask->Argument );
			CurrentTask->Status = TASK_STATUS_FINISHED;
			cnd_broadcast ( &Pool->TaskFinishedCondition );
			free ( CurrentTask );
			mtx_lock ( &Pool->TaskQueueMutex );
			PointerList_DestroyNode ( &Pool->TaskQueue, TaskNode );
			mtx_unlock ( &Pool->TaskQueueMutex );
			}
		else // No more tasks. Wait for a signal
			{
			atomic_fetch_add ( &Pool->FreeThreads, 1 );
			LOG_DEBUG ( "sleeping - %u %u %u", thrd_current(), Pool->FreeThreads, Pool->ThreadCount );
			mtx_lock ( &Pool->WakeUpMutex );
			cnd_wait ( &Pool->WakeUpCondition, &Pool->WakeUpMutex );
			mtx_unlock ( &Pool->WakeUpMutex ); // unlock mutex so that other ThreadArray can wait using it
			atomic_fetch_sub ( &Pool->FreeThreads, 1 );
			LOG_DEBUG ( "woke up - %u %u %u", thrd_current(), Pool->FreeThreads, Pool->ThreadCount );
			}
		}
	return 0;
	}

ThreadPool *ThreadPool_Create ( void )
	{
	ThreadPool *Pool = calloc ( 1, sizeof ( ThreadPool ) );
	if ( Pool == NULL )
		return NULL;

	unsigned Progress = 0;
	PointerList_Initialize ( &Pool->TaskQueue );
	Pool->LastTaskID = -1;
	if ( mtx_init ( &Pool->TaskQueueMutex, mtx_plain ) != thrd_success )
		goto OnError;
	++Progress;
	if ( cnd_init ( &Pool->WakeUpCondition ) != thrd_success )
		goto OnError;
	++Progress;
	if ( mtx_init ( &Pool->WakeUpMutex, mtx_plain ) != thrd_success )
		goto OnError;
	++Progress;
	if ( cnd_init ( &Pool->TaskFinishedCondition ) != thrd_success )
		goto OnError;
	++Progress;
	if ( mtx_init ( &Pool->TaskFinishedMutex, mtx_recursive ) != thrd_success )
		goto OnError;
	++Progress;

	Pool->ThreadCount = Platform_GetCoreCount();
	Pool->FreeThreads = 0;
	Pool->Quitting = false;
	Pool->ThreadArray = calloc ( Pool->ThreadCount, sizeof ( thrd_t ) );
	if ( Pool->ThreadArray == NULL )
		goto OnError;
	++Progress;

	for ( unsigned index = 0; index < Pool->ThreadCount; ++index )
		{
		thrd_create ( &Pool->ThreadArray[index], ( thrd_start_t ) ThreadPool_ThreadWorker, Pool );
		}
	return Pool;

OnError:
	if ( Progress >= 1 )
		mtx_destroy ( &Pool->TaskQueueMutex );
	if ( Progress >= 2 )
		cnd_destroy ( &Pool->WakeUpCondition );
	if ( Progress >= 3 )
		mtx_destroy ( &Pool->WakeUpMutex );
	if ( Progress >= 4 )
		cnd_destroy ( &Pool->TaskFinishedCondition );
	if ( Progress >= 5 )
		mtx_destroy ( &Pool->TaskFinishedMutex );
	SAFE_DEL_C ( Pool->ThreadArray );
	free ( Pool );
	return NULL;
	}

void ThreadPool_Destroy ( ThreadPool *Pool )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return;

	// Ask all the threads to join back
	Pool->Quitting = true;
	cnd_broadcast ( &Pool->WakeUpCondition );
	for ( unsigned index = 0; index < Pool->ThreadCount; ++index )
		{
		thrd_join ( Pool->ThreadArray[index], NULL );
		}

	ThreadPool_CancelAll ( Pool );
	Pool->ThreadCount = 0;
	free ( Pool->ThreadArray );
	mtx_destroy ( &Pool->TaskQueueMutex );
	cnd_destroy ( &Pool->WakeUpCondition );
	mtx_destroy ( &Pool->WakeUpMutex );
	cnd_destroy ( &Pool->TaskFinishedCondition );
	mtx_destroy ( &Pool->TaskFinishedMutex );

	Pool->LastTaskID = -1;
	PointerList_Clear ( &Pool->TaskQueue );
	free ( Pool );
	}

int ThreadPool_AddTask ( ThreadPool *Pool, int ( *FunctionPointer ) ( void * ), void *Argument )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return -1;

	TaskData *NewTask = ( TaskData * ) calloc ( 1, sizeof ( TaskData ) );
	if ( NewTask == NULL )
		return -2;

	mtx_lock ( &Pool->TaskQueueMutex );
	int TaskID = ++Pool->LastTaskID;
	NewTask->FunctionPointer = FunctionPointer;
	NewTask->Argument = Argument;
	NewTask->TaskID = TaskID;
	NewTask->Status = TASK_STATUS_QUEUED;

	PointerList_AddAtEnd ( &Pool->TaskQueue, NewTask );

	cnd_signal ( &Pool->WakeUpCondition );
	mtx_unlock ( &Pool->TaskQueueMutex );
	LOG_DEBUG ( "Added new task. %u %u %u", ThreadPool_GetTaskCount ( Pool ), Pool->FreeThreads, Pool->ThreadCount );
	return TaskID;
	}

void ThreadPool_CancelTask ( ThreadPool *Pool, const int TaskID )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return;
	if ( TaskID < 0 )
		return;

	// Search for this task on the queue
	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->TaskQueue ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData * ) PointerList_GetNodeData ( NodeIterator );
		if ( TaskPointer->TaskID == TaskID )
			{
			// If it's already running, don't delete it!
			if ( TaskPointer->Status == TASK_STATUS_RUNNING )
				break;
			free ( TaskPointer );
			cnd_broadcast ( &Pool->TaskFinishedCondition );
			PointerList_DestroyNode ( &Pool->TaskQueue, NodeIterator );
			break;
			}
		}
	mtx_unlock ( &Pool->TaskQueueMutex );
	}

void ThreadPool_CancelAll ( ThreadPool *Pool )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return;

	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->TaskQueue ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData * ) PointerList_GetNodeData ( NodeIterator );
		// If it's already running, don't delete it!
		if ( TaskPointer->Status == TASK_STATUS_RUNNING )
			continue;
		free ( TaskPointer );
		}
	PointerList_Clear ( &Pool->TaskQueue );
	mtx_unlock ( &Pool->TaskQueueMutex );
	}

TaskStatus ThreadPool_GetTaskStatus ( ThreadPool *Pool, const int TaskID )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return false;
	if ( TaskID < 0 )
		return false;

	// Search for this task on the queue
	TaskStatus Result = TASK_STATUS_NOT_FOUND;
	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->TaskQueue ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData * ) PointerList_GetNodeData ( NodeIterator );
		if ( TaskPointer->TaskID == TaskID )
			{
			Result = TaskPointer->Status;
			break;
			}
		}
	mtx_unlock ( &Pool->TaskQueueMutex );
	return Result;
	}

bool ThreadPool_WaitForAllTasks ( ThreadPool *Pool )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return false;

	// For some fucking stupid reason, cnd_timedwait does not take an interval, it takes an absolute time value
	struct timespec TimeToWait;
	timespec_get ( &TimeToWait, TIME_UTC ); // Get current time
	TimeToWait.tv_sec += 20;

	unsigned Size = ThreadPool_GetTaskCount ( Pool );
	unsigned FreeThreads = atomic_load ( &Pool->FreeThreads );
	LOG_DEBUG ( "Waiting for all tasks. %u %u", Size, FreeThreads );
	while ( ( Size > 0 ) || ( FreeThreads != Pool->ThreadCount ) )
		{
		LOG_DEBUG ( "Task queue size %u - Threads %u/%u", Size, FreeThreads, Pool->ThreadCount );
		mtx_lock ( &Pool->TaskFinishedMutex );
		int WaitResult = cnd_timedwait ( &Pool->TaskFinishedCondition, &Pool->TaskFinishedMutex, &TimeToWait );
		mtx_unlock ( &Pool->TaskFinishedMutex );

		if ( WaitResult == thrd_timedout )
			{
			LOG_ERROR ( "Timeout occurred while waiting for task finished condition" );
			return false;
			}
		Size = ThreadPool_GetTaskCount ( Pool );
		FreeThreads = atomic_load ( &Pool->FreeThreads );
		}
	LOG_DEBUG ( "Waiting complete" );

	return true;
	}
