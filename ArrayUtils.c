#include "ArrayUtils.h"
#include <stdlib.h>
#include <memory.h>

bool Array_Resize ( void **ArrayPointer, const unsigned Used, unsigned *Capacity, const unsigned ElementSize, const unsigned DesiredCapacity, const bool Clear )
	{
	if ( *Capacity == DesiredCapacity )
		return true;
	if ( Used > DesiredCapacity )
		return false;

	if ( Clear )
		{
		if ( Memory_ReallocAndClear ( ArrayPointer, *Capacity * ElementSize, DesiredCapacity * ElementSize ) == false )
			return false;
		}
	else
		{
		if ( Memory_Realloc ( ArrayPointer, DesiredCapacity * ElementSize ) == false )
			return false;
		}
	*Capacity = DesiredCapacity;
	return true;
	}

bool Array_EnsureFreeSpace ( void **ArrayPointer, const unsigned Used, unsigned *Capacity, const unsigned ElementSize, const unsigned DesiredFreeSpace, const bool Clear )
	{
	// If I already have enough free space, return
	if ( *Capacity >= Used + DesiredFreeSpace )
		return true;

	// Calculate new Capacity and realloc
	unsigned NewCapacity = Used + DesiredFreeSpace;
	return Array_Resize ( ArrayPointer, Used, Capacity, ElementSize, NewCapacity, Clear );
	}

bool Array_DeleteAndShift ( void **ArrayPointer, unsigned *Used, const unsigned ElementSize, const unsigned IndexToDelete )
	{
	if ( IndexToDelete >= *Used )
		return false;
	memmove ( ( char * ) * ArrayPointer + ( IndexToDelete * ElementSize ), ( char * ) * ArrayPointer + ( ( IndexToDelete + 1 ) * ElementSize ), ( *Used - IndexToDelete - 1 ) * ElementSize );
	-- ( *Used );
	return true;
	}

bool Array_DeleteAndSwap ( void **ArrayPointer, unsigned *Used, const unsigned ElementSize, const unsigned IndexToDelete )
	{
	if ( IndexToDelete >= *Used )
		return false;
	memcpy ( ( char * ) * ArrayPointer + ( IndexToDelete * ElementSize ), ( char * ) * ArrayPointer + ( ( *Used - 1 ) * ElementSize ), ElementSize );
	-- ( *Used );
	return true;
	}

void Array_Clear ( void **ArrayPointer, unsigned *Used, unsigned *Capacity, const unsigned ElementSize, const bool Clear )
	{
	if ( Clear )
		memset ( *ArrayPointer, 0, ElementSize * *Capacity );
	*Used = 0;
	}

void Array_Free ( void **ArrayPointer, unsigned *Used, unsigned *Capacity )
	{
	if ( *ArrayPointer )
		free ( *ArrayPointer );
	*ArrayPointer = NULL;
	*Used = 0;
	*Capacity = 0;
	}

bool Memory_ReallocAndClear ( void **Pointer, const unsigned CurrentSize, const unsigned NewSize )
	{
	if ( CurrentSize == NewSize )
		return true;

	void *NewPointer = realloc ( *Pointer, NewSize );
	if ( NewPointer == NULL )
		return false;
	*Pointer = NewPointer;
	// Clears the new area
	if ( NewSize > CurrentSize )
		memset ( ( char * ) * Pointer + CurrentSize, 0, NewSize - CurrentSize );
	return true;
	}

bool Memory_Realloc ( void **Pointer, const unsigned NewSize )
	{
	void *NewPointer = realloc ( *Pointer, NewSize );
	if ( NewPointer == NULL )
		return false;
	*Pointer = NewPointer;
	return true;
	}
