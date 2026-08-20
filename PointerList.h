#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <Platform/defines.h>

BEGIN_C_DECLARATIONS

typedef struct PointerListNode PointerListNode;
typedef struct PointerList PointerList;

typedef struct PointerListNode
	{
	PointerList *Owner;
	const void *Data;
	PointerListNode *Next, *Previous;
	} PointerListNode;

typedef struct PointerList
	{
	PointerListNode *First, *Last;
	unsigned Count;
	} PointerList;

void PointerList_Initialize ( PointerList *List );
void PointerList_Destroy ( PointerList *List );
PointerListNode *PointerList_AddAtEnd ( PointerList *List, const void *Data );
PointerListNode *PointerList_InsertAfter ( PointerList *List, PointerListNode *Node, const void *Data );
PointerListNode *PointerList_InsertBefore ( PointerList *List, PointerListNode *Node, const void *Data );
void PointerList_DestroyNode ( PointerList *List, PointerListNode *Node );

bool PointerList_IsEmpty ( const PointerList *List );
unsigned PointerList_GetSize ( const PointerList *List );
void PointerList_Clear ( PointerList *List );
const void *PointerList_GetNodeData ( const PointerListNode *Node );

PointerListNode *PointerList_Find ( const PointerList *List, PointerListNode *Start, const void *Data );
PointerListNode *PointerList_GetFirst ( const PointerList *List );
PointerListNode *PointerList_GetLast ( const PointerList *List );
PointerListNode *PointerList_GetNextNode ( const PointerListNode *Node );
PointerListNode *PointerList_GetPreviousNode ( const PointerListNode *Node );

END_C_DECLARATIONS
