#include "PointerArray.h"
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

bool PointerArray_Reserve ( PointerArray *Array, const unsigned NewCapacity )
	{
	if ( NewCapacity < Array->Count )
		return false;
	void *temp = realloc ( Array->Data, sizeof ( uintptr_t ) * NewCapacity );
	assert ( temp != NULL );
	if ( temp == NULL )
		return false;
	Array->Data = temp;
	Array->Capacity = NewCapacity;
	return true;
	}

bool PointerArray_EnsureFreeSpace ( PointerArray *Array, const unsigned FreeSpace )
	{
	unsigned NeededCapacity = Array->Count + FreeSpace;
	if ( NeededCapacity > Array->Capacity )
		{
		unsigned NewSize = NeededCapacity / 10;
		NewSize *= 10;
		if ( NewSize < NeededCapacity )
			NewSize += 10;
		return PointerArray_Reserve ( Array, NewSize );
		}
	return true;
	}

bool PointerArray_AddAtEnd ( PointerArray *Array, const intptr_t Data )
	{
	if ( PointerArray_EnsureFreeSpace ( Array, 1 ) == false )
		return false;

	Array->Data[Array->Count] = Data;
	++Array->Count;
	return true;
	}

bool PointerArray_InsertAt ( PointerArray *Array, const unsigned Index, const intptr_t Data )
	{
	if ( Index > Array->Count )
		return false;
	if ( PointerArray_EnsureFreeSpace ( Array, 1 ) == false )
		return false;

	// shift all elements forward
	memmove ( Array->Data + Index + 1, Array->Data + Index, ( Array->Count - Index ) * sizeof ( intptr_t ) );
	Array->Data[Index] = Data;
	++Array->Count;
	return true;
	}

void PointerArray_RemoveAt ( PointerArray *Array, const unsigned Index )
	{
	if ( Index >= Array->Count )
		return;

	// shift all elements back
	memmove ( Array->Data + Index, Array->Data + Index + 1, ( Array->Count - Index - 1 ) * sizeof ( intptr_t ) );
	--Array->Count;
	}

bool PointerArray_IsEmpty ( const PointerArray *Array )
	{
	return Array->Count == 0;
	}

unsigned PointerArray_GetSize ( const PointerArray *Array )
	{
	return Array->Count;
	}

void PointerArray_Clear ( PointerArray *Array )
	{
	Array->Count = 0;
	}

intptr_t PointerArray_Get ( const PointerArray *Array, const unsigned Index )
	{
	return Array->Data[Index];
	}

void PointerArray_Set ( const PointerArray *Array, const unsigned Index, const intptr_t NewValue )
	{
	assert ( Index < Array->Count );
	Array->Data[Index] = NewValue;
	}
