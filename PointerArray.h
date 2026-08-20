#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <Platform/defines.h>

BEGIN_C_DECLARATIONS
typedef struct
	{
	intptr_t *Data;
	unsigned Count, Capacity;
	} PointerArray;

void PointerArray_Initialize ( PointerArray *Array );
void PointerArray_Destroy ( PointerArray *Array );
bool PointerArray_Reserve ( PointerArray *Array, unsigned NewCapacity );
bool PointerArray_EnsureFreeSpace ( PointerArray *Array, unsigned FreeSpace );
bool PointerArray_AddAtEnd ( PointerArray *Array, intptr_t Data );
bool PointerArray_InsertAt ( PointerArray *Array, unsigned Index, intptr_t Data );
void PointerArray_RemoveAt ( PointerArray *Array, unsigned Index );
bool PointerArray_IsEmpty ( const PointerArray *Array );
unsigned PointerArray_GetSize ( const PointerArray *Array );
void PointerArray_Clear ( PointerArray *Array );
intptr_t PointerArray_Get ( const PointerArray *Array, const unsigned Index );
void PointerArray_Set ( const PointerArray *Array, const unsigned Index, const intptr_t NewValue );
END_C_DECLARATIONS
