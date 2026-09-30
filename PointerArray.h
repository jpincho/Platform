#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "Platform.h"

BEGIN_C_DECLARATIONS
typedef struct
	{
	void **Data;
	unsigned Count, Capacity;
	} PointerArray;

void PointerArray_Initialize ( PointerArray *Array );
void PointerArray_Destroy ( PointerArray *Array );
bool PointerArray_Reserve ( PointerArray *Array, const unsigned NewCapacity );
bool PointerArray_EnsureFreeSpace ( PointerArray *Array, const unsigned FreeSpace );
bool PointerArray_AddAtEnd ( PointerArray *Array, const void *Data );
bool PointerArray_InsertAt ( PointerArray *Array, const unsigned Index, const void *Data );
void PointerArray_RemoveAt ( PointerArray *Array, const unsigned Index );
bool PointerArray_IsEmpty ( const PointerArray *Array );
int PointerArray_Find ( PointerArray *Array, const void *Data );
unsigned PointerArray_GetSize ( const PointerArray *Array );
void PointerArray_Clear ( PointerArray *Array );
void *PointerArray_Get ( const PointerArray *Array, const unsigned Index );
void PointerArray_Set ( const PointerArray *Array, const unsigned Index, const void *Data );
END_C_DECLARATIONS
