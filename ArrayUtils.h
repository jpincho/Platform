#pragma once
#include <stdbool.h>

bool Array_Resize ( void **ArrayPointer, const unsigned Used, unsigned *Capacity, const unsigned ElementSize, const unsigned DesiredCapacity, const bool Clear );
bool Array_EnsureFreeSpace ( void **ArrayPointer, const unsigned Used, unsigned *Capacity, const unsigned ElementSize, const unsigned DesiredFreeSpace, const bool Clear );
bool Array_DeleteAndShift ( void **ArrayPointer, unsigned *Used, const unsigned ElementSize, const unsigned IndexToDelete );
bool Array_DeleteAndSwap ( void **ArrayPointer, unsigned *Used, const unsigned ElementSize, const unsigned IndexToDelete );
void Array_Clear ( void **ArrayPointer, unsigned *Used, unsigned *Capacity, const unsigned ElementSize, const bool Clear );
void Array_Free ( void **ArrayPointer, unsigned *Used, unsigned *Capacity );
bool Memory_ReallocAndClear ( void **Pointer, const unsigned CurrentSize, const unsigned NewSize );
bool Memory_Realloc ( void **Pointer, const unsigned NewSize );

#define ARRAY_RESIZE(ARRAY,USED,CAPACITY,DESIRED)               Array_Resize            ((void**)&ARRAY,USED,&CAPACITY,sizeof(ARRAY[0]),DESIRED,true)
#define ARRAY_ENSURE_FREE_SPACE(ARRAY,USED,CAPACITY,DESIRED)    Array_EnsureFreeSpace   ((void**)&ARRAY,USED,&CAPACITY,sizeof(ARRAY[0]),DESIRED,true)
#define ARRAY_PUSH_BACK(ARRAY,USED,CAPACITY,ITEM)               {Array_EnsureFreeSpace  ((void**)&ARRAY,USED,&CAPACITY,sizeof(ARRAY[0]), 1, false);ARRAY[USED]=ITEM;++USED; }
#define ARRAY_DELETE_AND_SWAP(ARRAY,USED,INDEX)                 Array_DeleteAndSwap     ((void**)&ARRAY,&USED,sizeof(ARRAY[0]), INDEX);