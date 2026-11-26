#include "PointerArray.h"
#include "ArrayUtils.h"
#include <stdlib.h>
#include <assert.h>

#include <Platform/Platform.h>
#if defined ( PLATFORM_COMPILER_GNU)
#include <string.h>
#endif

void PointerArray_Initialize ( PointerArray *Array )
	{
	Array->Capacity = Array->Count = 0;
	Array->Data = NULL;
	}

void PointerArray_Destroy ( PointerArray *Array )
	{
	Array->Capacity = Array->Count = 0;
	if ( Array->Data )
		free ( Array->Data );
	Array->Data = NULL;
	}

bool PointerArray_Reserve ( PointerArray *Array, const size_t NewCapacity )
	{
	if ( NewCapacity < Array->Count )
		return false;
	return Array_Resize ( ( void** ) &Array->Data, Array->Count, &Array->Capacity, sizeof ( void* ), NewCapacity, false );
	}

bool PointerArray_EnsureFreeSpace ( PointerArray *Array, const size_t FreeSpace )
	{
	return Array_EnsureFreeSpace ( ( void** ) &Array->Data, Array->Count, &Array->Capacity, sizeof ( void* ), FreeSpace, false );
	}

bool PointerArray_AddAtEnd ( PointerArray *Array, const void *Data )
	{
	if ( PointerArray_EnsureFreeSpace ( Array, 1 ) == false )
		return false;

	Array->Data[Array->Count] = Data;
	++Array->Count;
	return true;
	}

bool PointerArray_InsertAt ( PointerArray *Array, const size_t Index, const void *Data )
	{
	if ( Index > Array->Count )
		return false;

	if ( Array_ShiftRight ( ( void * * ) &Array->Data, &Array->Count, &Array->Capacity, sizeof ( void * ), Index, 1 ) == false )
		return false;
	Array->Data[Index] = Data;
	return true;
	}

void PointerArray_RemoveAndShift ( PointerArray *Array, const size_t Index )
	{
	if ( Index >= Array->Count )
		return;

	Array_RemoveAndShift ( ( void** ) &Array->Data, &Array->Count, sizeof ( void* ), Index );
	}

void PointerArray_RemoveAndSwap ( PointerArray *Array, const size_t Index )
	{
	if ( Index >= Array->Count )
		return;

	Array_RemoveAndSwap ( ( void** ) &Array->Data, &Array->Count, sizeof ( void* ), Index );
	}

bool PointerArray_IsEmpty ( const PointerArray *Array )
	{
	return Array->Count == 0;
	}

int PointerArray_Find ( PointerArray *Array, const void *Data )
	{
	for ( unsigned Index = 0; Index < Array->Count; ++Index )
		{
		if ( Array->Data[Index] == Data )
			return Index;
		}
	return -1;
	}

size_t PointerArray_GetSize ( const PointerArray *Array )
	{
	return Array->Count;
	}

void PointerArray_Clear ( PointerArray *Array )
	{
	Array->Count = 0;
	}

const void *PointerArray_Get ( const PointerArray *Array, const size_t Index )
	{
	return Array->Data[Index];
	}

void PointerArray_Set ( const PointerArray *Array, const size_t Index, const void *NewData )
	{
	assert ( Index < Array->Count );
	Array->Data[Index] = NewData;
	}
