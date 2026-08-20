#pragma once
#include <stdbool.h>

typedef struct ThreadPool ThreadPool;

typedef enum
	{
	TASK_STATUS_NOT_FOUND,
	TASK_STATUS_QUEUED,
	TASK_STATUS_RUNNING,
	TASK_STATUS_FINISHED
	} TaskStatus;

ThreadPool *ThreadPool_Create ( const unsigned ThreadCount );
void ThreadPool_Destroy ( ThreadPool *Pool );
unsigned ThreadPool_GetThreadCount ( const ThreadPool *Pool );
int ThreadPool_AddTask ( ThreadPool *Pool, int ( *FunctionPointer ) ( void * ), void *Argument, const bool Keep );
void ThreadPool_CancelTask ( ThreadPool *Pool, const int TaskID );
void ThreadPool_CancelAllTasks ( ThreadPool *Pool );
TaskStatus ThreadPool_GetTaskStatus ( ThreadPool *Pool, const int TaskID );
bool ThreadPool_WaitForAllTasks ( ThreadPool *Pool );

bool ThreadPool_WaitForTask ( ThreadPool *Pool, const int TaskID );
bool ThreadPool_FreeKeptTask ( ThreadPool *Pool, const int TaskID );
void ThreadPool_FreeAllFinishedKeptTasks ( ThreadPool *Pool );