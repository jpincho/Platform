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

ThreadPool *ThreadPool_Create ( void );
void ThreadPool_Destroy ( ThreadPool *Pool );
int ThreadPool_AddTask ( ThreadPool *Pool, int ( *FunctionPointer ) ( void * ), void *Argument );
void ThreadPool_CancelTask ( ThreadPool *Pool, const int TaskID );
void ThreadPool_CancelAll ( ThreadPool *Pool );
TaskStatus ThreadPool_GetTaskStatus ( ThreadPool *Pool, const int TaskID );
bool ThreadPool_WaitForAllTasks ( ThreadPool *Pool );
