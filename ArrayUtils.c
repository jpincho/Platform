#include "ArrayUtils.h"
#include <memory.h>

bool Array_Resize ( void **ArrayPointer, const size_t Used, size_t *Capacity, const size_t ElementSize, const size_t DesiredCapacity, const bool Clear )
	{
	if ( *Capacity == DesiredCapacity )
		return true;
	if ( Used > DesiredCapacity )
		return false;
	if ( DesiredCapacity == 0 )
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

bool Array_EnsureFreeSpace ( void **ArrayPointer, const size_t Used, size_t *Capacity, const size_t ElementSize, const size_t DesiredFreeSpace, const bool Clear )
	{
	// If I already have enough free space, return
	if ( *Capacity >= Used + DesiredFreeSpace )
		return true;

	// Calculate new Capacity and realloc
	size_t NewCapacity = Used + DesiredFreeSpace;
	return Array_Resize ( ArrayPointer, Used, Capacity, ElementSize, NewCapacity, Clear );
	}

bool Array_ShiftRight ( void **ArrayPointer, size_t *Used, size_t *Capacity, const size_t ElementSize, const size_t IndexToShift, const size_t ShiftCount )
	{
	if ( IndexToShift > *Used )
		return false;
	if ( ShiftCount == 0 )
		return true;
	if ( Array_EnsureFreeSpace ( ArrayPointer, *Used, Capacity, ElementSize, ShiftCount, false ) == false )
		return false;
	if ( IndexToShift != *Used )
		memmove ( ( char * ) * ArrayPointer + ( ( IndexToShift + ShiftCount ) * ElementSize ), ( char * ) * ArrayPointer + ( IndexToShift * ElementSize ), ( *Used - IndexToShift ) * ElementSize );
	( *Used ) += ShiftCount;
	return true;
	}

bool Array_RemoveAndShift ( void **ArrayPointer, size_t *Used, const size_t ElementSize, const size_t IndexToRemove )
	{
	if ( IndexToRemove >= *Used )
		return false;
	if ( IndexToRemove != *Used - 1 )
		memmove ( ( char * ) * ArrayPointer + ( IndexToRemove * ElementSize ), ( char * ) * ArrayPointer + ( ( IndexToRemove + 1 ) * ElementSize ), ( *Used - IndexToRemove - 1 ) * ElementSize );
	-- ( *Used );
	return true;
	}

bool Array_RemoveAndSwap ( void **ArrayPointer, size_t *Used, const size_t ElementSize, const size_t IndexToRemove )
	{
	if ( IndexToRemove >= *Used )
		return false;
	if ( IndexToRemove != *Used - 1 )
		memcpy ( ( char * ) * ArrayPointer + ( IndexToRemove * ElementSize ), ( char * ) * ArrayPointer + ( ( *Used - 1 ) * ElementSize ), ElementSize );
	-- ( *Used );
	return true;
	}

void Array_Clear ( void **ArrayPointer, size_t *Used, size_t *Capacity, const size_t ElementSize, const bool Clear )
	{
	if ( ( Clear ) && ( *ArrayPointer != NULL ) )
		memset ( *ArrayPointer, 0, ElementSize * *Capacity );
	*Used = 0;
	}

void Array_Free ( void **ArrayPointer, size_t *Used, size_t *Capacity )
	{
	if ( *ArrayPointer != NULL )
		free ( *ArrayPointer );
	*ArrayPointer = NULL;
	*Used = 0;
	*Capacity = 0;
	}

bool Memory_ReallocAndClear ( void **Pointer, const size_t CurrentSize, const size_t NewSize )
	{
	if ( CurrentSize == NewSize )
		return true;
	if ( NewSize == 0 )
		return false;
	void *NewPointer = realloc ( *Pointer, NewSize );
	if ( NewPointer == NULL )
		return false;
	*Pointer = NewPointer;
	// Clears the new area
	if ( NewSize > CurrentSize )
		memset ( ( char * ) * Pointer + CurrentSize, 0, NewSize - CurrentSize );
	return true;
	}

bool Memory_Realloc ( void **Pointer, const size_t NewSize )
	{
	if ( NewSize == 0 )
		return false;
	void *NewPointer = realloc ( *Pointer, NewSize );
	if ( NewPointer == NULL )
		return false;
	*Pointer = NewPointer;
	return true;
	}
