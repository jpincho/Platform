#pragma once
#include <stdbool.h>
#include <stdlib.h>

bool Array_Resize ( void **ArrayPointer, const size_t Used, size_t *Capacity, const size_t ElementSize, const size_t DesiredCapacity, const bool Clear );
bool Array_EnsureFreeSpace ( void **ArrayPointer, const size_t Used, size_t *Capacity, const size_t ElementSize, const size_t DesiredFreeSpace, const bool Clear );
bool Array_ShiftRight ( void **ArrayPointer, size_t *Used, size_t *Capacity, const size_t ElementSize, const size_t IndexToShift, const size_t ShiftCount );
bool Array_RemoveAndShift ( void **ArrayPointer, size_t *Used, const size_t ElementSize, const size_t IndexToRemove );
bool Array_RemoveAndSwap ( void **ArrayPointer, size_t *Used, const size_t ElementSize, const size_t IndexToRemove );
void Array_Clear ( void **ArrayPointer, size_t *Used, size_t *Capacity, const size_t ElementSize, const bool Clear );
void Array_Free ( void **ArrayPointer, size_t *Used, size_t *Capacity );
bool Memory_ReallocAndClear ( void **Pointer, const size_t CurrentSize, const size_t NewSize );
bool Memory_Realloc ( void **Pointer, const size_t NewSize );
