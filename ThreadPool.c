#include "ThreadPool.h"
#include "PointerList.h"
#include <threads.h>
#include <assert.h>

typedef struct
	{
	int ( *FunctionPointer ) ( void * );
	void *Argument;
	int TaskID;
	int Result;
	TaskStatus Status;
	bool Keep;
	} TaskData;

typedef struct ThreadPool
	{
	PointerList TaskQueue, KeptTasks;

	int LastTaskID;
	mtx_t TaskQueueMutex;
	cnd_t WakeUpCondition;
	mtx_t WakeUpMutex;
	cnd_t TaskFinishedCondition;
	mtx_t TaskFinishedMutex;

	thrd_t *ThreadArray;
	unsigned ThreadCount;
	bool Quitting;
	} ThreadPool;

static int ThreadPool_LoopFunction ( ThreadPool *Pool )
	{
	do
		{
		// Grab the first available task, if available
		mtx_lock ( &Pool->TaskQueueMutex );
		PointerListNode *TaskNode = PointerList_GetFirst ( &Pool->TaskQueue );
		TaskData *CurrentTask = NULL;
		if ( TaskNode != NULL )
			{
			CurrentTask = ( TaskData* ) PointerList_GetNodeData ( TaskNode );
			PointerList_DestroyNode ( &Pool->TaskQueue, TaskNode );
			}
		mtx_unlock ( &Pool->TaskQueueMutex );

		if ( CurrentTask != NULL ) // There was a task. run it...
			{
			CurrentTask->Status = TASK_STATUS_RUNNING;
			CurrentTask->Result = CurrentTask->FunctionPointer ( CurrentTask->Argument );
			CurrentTask->Status = TASK_STATUS_FINISHED;
			cnd_signal ( &Pool->TaskFinishedCondition );
			if ( CurrentTask->Keep == false )
				free ( CurrentTask );
			}
		else // No more tasks. Wait for a signal
			{
			mtx_lock ( &Pool->WakeUpMutex );
			cnd_wait ( &Pool->WakeUpCondition, &Pool->WakeUpMutex );
			mtx_unlock ( &Pool->WakeUpMutex ); // unlock mutex so that other ThreadArray can wait using it
			}
		}
	while ( Pool->Quitting == false );
	return 0;
	}

ThreadPool *ThreadPool_Create ( const unsigned ThreadCount )
	{
	if ( ThreadCount == 0 )
		return NULL;
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

	Pool->ThreadCount = ThreadCount;
	Pool->Quitting = false;
	Pool->ThreadArray = calloc ( Pool->ThreadCount, sizeof ( thrd_t ) );
	if ( Pool->ThreadArray == NULL )
		goto OnError;
	++Progress;

	for ( unsigned index = 0; index < Pool->ThreadCount; ++index )
		{
		thrd_create ( &Pool->ThreadArray[index], ( thrd_start_t ) ThreadPool_LoopFunction, Pool );
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

	ThreadPool_CancelAllTasks ( Pool );

	Pool->Quitting = true;
	cnd_broadcast ( &Pool->WakeUpCondition );

	for ( unsigned index = 0; index < Pool->ThreadCount; ++index )
		{
		thrd_join ( Pool->ThreadArray[index], NULL );
		}

	Pool->ThreadCount = 0;
	free ( Pool->ThreadArray );
	mtx_destroy ( &Pool->TaskQueueMutex );
	cnd_destroy ( &Pool->WakeUpCondition );
	mtx_destroy ( &Pool->WakeUpMutex );
	cnd_destroy ( &Pool->TaskFinishedCondition );
	mtx_destroy ( &Pool->TaskFinishedMutex );

	Pool->LastTaskID = -1;
	PointerList_Destroy ( &Pool->TaskQueue );
	PointerList_Destroy ( &Pool->KeptTasks );
	free ( Pool );
	}

unsigned ThreadPool_GetThreadCount ( const ThreadPool *Pool )
	{
	if ( Pool == NULL )
		return 0;
	return Pool->ThreadCount;
	}

int ThreadPool_AddTask ( ThreadPool *Pool, int ( *FunctionPointer ) ( void * ), void *Argument, const bool Keep )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return -1;

	TaskData *NewTask = ( TaskData * ) calloc ( 1, sizeof ( TaskData ) );
	if ( NewTask == NULL )
		return -2;

	int TaskID = ++Pool->LastTaskID;
	NewTask->FunctionPointer = FunctionPointer;
	NewTask->Argument = Argument;
	NewTask->TaskID = TaskID;
	NewTask->Status = TASK_STATUS_QUEUED;
	NewTask->Keep = Keep;

	mtx_lock ( &Pool->TaskQueueMutex );
	PointerList_AddAtEnd ( &Pool->TaskQueue, NewTask );
	if ( Keep )
		PointerList_AddAtEnd ( &Pool->KeptTasks, NewTask );
	mtx_unlock ( &Pool->TaskQueueMutex );

	cnd_signal ( &Pool->WakeUpCondition );
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
		TaskData *TaskPointer = ( TaskData* ) PointerList_GetNodeData ( NodeIterator );
		if ( TaskPointer->TaskID == TaskID )
			{
			if ( TaskPointer->Keep == true )
				{
				for ( PointerListNode *KeptNodeIterator = PointerList_GetFirst ( &Pool->KeptTasks ); KeptNodeIterator != NULL; KeptNodeIterator = PointerList_GetNextNode ( KeptNodeIterator ) )
					{
					TaskData *KeptTaskPointer = ( TaskData* ) PointerList_GetNodeData ( KeptNodeIterator );
					if ( KeptTaskPointer == TaskPointer )
						{
						PointerList_DestroyNode ( &Pool->KeptTasks, KeptNodeIterator );
						break;
						}
					}
				}
			free ( TaskPointer );
			PointerList_DestroyNode ( &Pool->TaskQueue, NodeIterator );
			break;
			}
		}
	mtx_unlock ( &Pool->TaskQueueMutex );
	}

void ThreadPool_CancelAllTasks ( ThreadPool *Pool )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return;

	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->TaskQueue ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData* ) PointerList_GetNodeData ( NodeIterator );
		free ( TaskPointer );
		}
	PointerList_Clear ( &Pool->TaskQueue );
	PointerList_Clear ( &Pool->KeptTasks );
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
	TaskStatus Status = TASK_STATUS_NOT_FOUND;
	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->TaskQueue ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData* ) PointerList_GetNodeData ( NodeIterator );
		if ( TaskPointer->TaskID == TaskID )
			{
			Status = TaskPointer->Status;
			break;
			}
		}
	if ( Status == TASK_STATUS_NOT_FOUND )
		{
		for ( PointerListNode *KeptNodeIterator = PointerList_GetFirst ( &Pool->KeptTasks ); KeptNodeIterator != NULL; KeptNodeIterator = PointerList_GetNextNode ( KeptNodeIterator ) )
			{
			TaskData *KeptTaskPointer = ( TaskData* ) PointerList_GetNodeData ( KeptNodeIterator );
			if ( KeptTaskPointer->TaskID == TaskID )
				{
				Status = KeptTaskPointer->Status;
				break;
				}
			}

		}
	mtx_unlock ( &Pool->TaskQueueMutex );
	return Status;
	}

bool ThreadPool_WaitForAllTasks ( ThreadPool *Pool )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return false;

	bool IsEmpty = false;
	mtx_lock ( &Pool->TaskQueueMutex );
	IsEmpty = PointerList_IsEmpty ( &Pool->TaskQueue );
	mtx_unlock ( &Pool->TaskQueueMutex );
	while ( IsEmpty == false )
		{
		mtx_lock ( &Pool->TaskFinishedMutex );
		cnd_wait ( &Pool->TaskFinishedCondition, &Pool->TaskFinishedMutex );
		mtx_unlock ( &Pool->TaskFinishedMutex );
		mtx_lock ( &Pool->TaskQueueMutex );
		IsEmpty = PointerList_IsEmpty ( &Pool->TaskQueue );
		mtx_unlock ( &Pool->TaskQueueMutex );
		}

	return true;
	}

bool ThreadPool_WaitForTask ( ThreadPool *Pool, const int TaskID )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return false;
	if ( TaskID < 0 )
		return false;

	// Search for this task on the queue
	TaskData *Task = NULL;
	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->TaskQueue ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData* ) PointerList_GetNodeData ( NodeIterator );
		if ( TaskPointer->TaskID == TaskID )
			{
			Task = TaskPointer;
			Task->Keep = true;
			break;
			}
		}
	mtx_unlock ( &Pool->TaskQueueMutex );

	if ( Task == NULL )
		return false;

	while ( Task->Status != TASK_STATUS_FINISHED )
		{
		mtx_lock ( &Pool->TaskFinishedMutex );
		cnd_wait ( &Pool->TaskFinishedCondition, &Pool->TaskFinishedMutex );
		mtx_unlock ( &Pool->TaskFinishedMutex );
		}
	return true;
	}

bool ThreadPool_FreeKeptTask ( ThreadPool *Pool, const int TaskID )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return false;
	if ( TaskID < 0 )
		return false;

	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->KeptTasks ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData* ) PointerList_GetNodeData ( NodeIterator );
		if ( TaskPointer->TaskID == TaskID )
			{
			if ( TaskPointer->Status == TASK_STATUS_FINISHED )
				{
				free ( TaskPointer );
				PointerList_DestroyNode ( &Pool->KeptTasks, NodeIterator );
				mtx_unlock ( &Pool->TaskQueueMutex );
				return true;
				}
			mtx_unlock ( &Pool->TaskQueueMutex );
			return false;
			}
		}
	mtx_unlock ( &Pool->TaskQueueMutex );
	return false;
	}

void ThreadPool_FreeAllFinishedKeptTasks ( ThreadPool *Pool )
	{
	assert ( Pool != NULL );
	if ( Pool == NULL )
		return;

	mtx_lock ( &Pool->TaskQueueMutex );
	for ( PointerListNode *NodeIterator = PointerList_GetFirst ( &Pool->KeptTasks ); NodeIterator != NULL; NodeIterator = PointerList_GetNextNode ( NodeIterator ) )
		{
		TaskData *TaskPointer = ( TaskData* ) PointerList_GetNodeData ( NodeIterator );
		if ( TaskPointer->Status == TASK_STATUS_FINISHED )
			{
			free ( TaskPointer );
			PointerList_DestroyNode ( &Pool->KeptTasks, NodeIterator );
			}
		}
	mtx_unlock ( &Pool->TaskQueueMutex );
	return;
	}