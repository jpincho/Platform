#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "Platform.h"

BEGIN_C_DECLARATIONS
typedef struct
	{
	const void **Data;
	size_t Count, Capacity;
	} PointerArray;

void PointerArray_Initialize ( PointerArray *Array );
void PointerArray_Destroy ( PointerArray *Array );
bool PointerArray_Reserve ( PointerArray *Array, const size_t NewCapacity );
bool PointerArray_EnsureFreeSpace ( PointerArray *Array, const size_t FreeSpace );
bool PointerArray_AddAtEnd ( PointerArray *Array, const void *Data );
bool PointerArray_InsertAt ( PointerArray *Array, const size_t Index, const void *Data );
void PointerArray_RemoveAndShift ( PointerArray *Array, const size_t Index );
void PointerArray_RemoveAndSwap ( PointerArray *Array, const size_t Index );
bool PointerArray_IsEmpty ( const PointerArray *Array );
int PointerArray_Find ( PointerArray *Array, const void *Data );
size_t PointerArray_GetSize ( const PointerArray *Array );
void PointerArray_Clear ( PointerArray *Array );
const void *PointerArray_Get ( const PointerArray *Array, const size_t Index );
void PointerArray_Set ( const PointerArray *Array, const size_t Index, const void *Data );
END_C_DECLARATIONS
