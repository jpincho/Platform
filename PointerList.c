#include "PointerList.h"
#include <memory.h>
#include <assert.h>

// public functions
void PointerList_Initialize ( PointerList *List )
	{
	List->First = List->Last = NULL;
	List->Count = 0;
	}

void PointerList_Destroy ( PointerList *List )
	{
	PointerList_Clear ( List );
	free ( List );
	}

PointerListNode *PointerList_AddAtEnd ( PointerList *List, const void *Data )
	{
	PointerListNode *NewNode = calloc ( 1, sizeof ( PointerListNode ) );
	if ( NewNode == NULL )
		return NULL;

	NewNode->Owner = List;
	NewNode->Next = NULL;
	NewNode->Previous = List->Last;
	NewNode->Data = Data;

	if ( List->First == NULL )
		{
		List->First = NewNode;
		}
	else
		{
		List->Last->Next = NewNode;
		}
	List->Last = NewNode;
	++List->Count;
	return NewNode;
	}

PointerListNode *PointerList_InsertAfter ( PointerList *List, PointerListNode *Node, const void *Data )
	{
	// In case the List is empty, <Node> parameter can be null
	if ( Node == NULL )
		{
		if ( List->First == NULL )
			return PointerList_AddAtEnd ( List, Data );
		else
			return NULL;
		}

	assert ( Node->Owner == List );
	PointerListNode *NewNode = calloc ( 1, sizeof ( PointerListNode ) );
	if ( NewNode == NULL )
		return NULL;

	// Connect this Node to neighbours
	NewNode->Owner = List;
	NewNode->Next = Node->Next;
	NewNode->Previous = Node;
	NewNode->Data = Data;

	// connect the reference Node and neighbours to the new Node
	if ( Node->Next != NULL )
		Node->Next->Previous = NewNode;
	Node->Next = NewNode;

	// if Last Node was the reference Node, it is now the new one
	if ( Node->Next == List->Last )
		List->Last = NewNode;
	++List->Count;
	return NewNode;
	}

PointerListNode *PointerList_InsertBefore ( PointerList *List, PointerListNode *Node, const void *Data )
	{
	// In case the List is empty, <Node> parameter can be null
	if ( Node == NULL )
		{
		if ( List->First == NULL )
			return PointerList_AddAtEnd ( List, Data );
		else
			return NULL;
		}

	assert ( Node->Owner == List );
	PointerListNode *NewNode = calloc ( 1, sizeof ( PointerListNode ) );
	if ( NewNode == NULL )
		return NULL;

	// Connect this Node to neighbours
	NewNode->Owner = List;
	NewNode->Next = Node;
	NewNode->Previous = Node->Previous;
	NewNode->Data = Data;

	// connect the reference Node and neighbours to the new Node
	if ( Node->Previous != NULL )
		Node->Previous->Next = NewNode;
	Node->Previous = NewNode;

	// if First Node was the reference Node, it is now the new one
	if ( Node->Previous == List->First )
		List->First = NewNode;
	++List->Count;
	return NewNode;
	}

void PointerList_DestroyNode ( PointerList *List, PointerListNode *Node )
	{
	assert ( Node->Owner == List );
	if ( Node->Previous )
		Node->Previous->Next = Node->Next;
	if ( Node->Next )
		Node->Next->Previous = Node->Previous;

	if ( Node == List->First )
		List->First = Node->Next;
	if ( Node == List->Last )
		List->Last = Node->Previous;
	Node->Next = Node->Previous = NULL;
	free ( Node );
	--List->Count;
	}

bool PointerList_IsEmpty ( const PointerList *List )
	{
	return List->Count == 0;
	}

unsigned PointerList_GetSize ( const PointerList *List )
	{
	return List->Count;
	}

void PointerList_Clear ( PointerList *List )
	{
	PointerListNode *Iterator = List->First;
	while ( Iterator != NULL )
		{
		PointerListNode *Next = Iterator->Next;
		Iterator->Next = Iterator->Previous = NULL;
		Iterator->Owner = NULL;
		free ( Iterator );
		Iterator = Next;
		}
	List->First = List->Last = NULL;
	List->Count = 0;
	}

const void *PointerList_GetNodeData ( const PointerListNode *Node )
	{
#define PLATFORM_DEBUG 1
#if defined (PLATFORM_DEBUG)
	assert ( Node->Owner != NULL );
	for ( PointerListNode *Iterator = Node->Owner->First; Iterator != NULL; Iterator = Iterator->Next )
		{
		if ( Iterator == Node )
			return Node->Data;
		}
	assert ( false && "Node does not belong to the List" );
#endif
	return Node->Data;
	}

PointerListNode *PointerList_Find ( const PointerList *List, PointerListNode *Start, const void *Data )
	{
	if ( Start == NULL )
		Start = List->First;
	if ( Start == NULL )
		return NULL;
	assert ( Start->Owner == List );
	PointerListNode *Iterator = Start;
	while ( ( Iterator != NULL ) && ( Iterator->Data != Data ) )
		{
		Iterator = Iterator->Next;
		}
	return Iterator;
	}

PointerListNode *PointerList_GetFirst ( const PointerList *List )
	{
	return List->First;
	}

PointerListNode *PointerList_GetLast ( const PointerList *List )
	{
	return List->Last;
	}

PointerListNode *PointerList_GetNextNode ( const PointerListNode *Node )
	{
	return Node->Next;
	}

PointerListNode *PointerList_GetPreviousNode ( const PointerListNode *Node )
	{
	return Node->Previous;
	}
