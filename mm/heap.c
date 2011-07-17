#include <sddk.h>
#include "mm.h"

MEMNODE*	HeapBase;
ULONG		HeapEnd;

VOID MmInitializeHeap( ULONG MemSize )
{
	HeapBase = (MEMNODE*)MmAllocatePageAlignedMemory( MemSize );
	HeapBase->Signature = MEM_SIGNATURE;
	HeapBase->Size = 0;
	HeapBase->Next = NULL;
	HeapBase->Previous = NULL;

	HeapEnd = (ULONG)HeapBase + MemSize;
}

PVOID
MmHeapAllocate( QWORD Size,
			    BYTE Type )
{
	PMM_HEAP_NODE pNew;
	PMM_HEAP_NODE pCurrent;
	PVOID Address = NULL;

	pCurrent = HeapBase;

	while( pCurrent != NULL )
	{
		if( pCurrent->Next == NULL )
		{
			if( ((QWORD)pCurrent + (2 * sizeof(MM_HEAP_NODE)) + pCurrent->Size + Size) >= HeapEnd ) return NULL;

			pNew = (PMM_HEAP_NODE)((QWORD)pCurrent + sizeof(MM_HEAP_NODE) + pCurrent->Size);
			pNew->Signature = MM_SIGNATURE;
			pNew->Size = Size;
			pNew->Next = NULL;
			pNew->Previous = pCurrent;

			pCurrent->Next = pNew;

			Address = (PVOID)((QWORD)pNew + sizeof(MM_HEAP_NODE));

			break;
		}

		if( ((QWORD)pCurrent + (2 * sizeof(MM_HEAP_NODE)) + pCurrent->Size + Size) < (QWORD)pCurrent->Next )
		{
			pNew = (PMM_HEAP_NODE)((QWORD)pCurrent + sizeof(MM_HEAP_NODE) + pCurrent->Size);
			pNew->Signature = MM_SIGNATURE;
			pNew->Size = Size;
			pNew->Next = pCurrent->Next;
			pNew->Previous = pCurrent;

			pCurrent->Next->Previous = pNew;
			pCurrent->Next = pNew;

			Address = (PVOID)(pNew + sizeof(MM_HEAP_NODE));

			break;
		}

		pCurrent = pCurrent->Next;
	}

	return Address;
}

VOID
MmHeapFree( PVOID Memory,
		    BYTE Type )
{
	PMM_HEAP_NODE node;

	if( (QWORD)Memory < ((QWORD)HeapBase + sizeof(MM_HEAP_NODE)) ) return;

	node = (PMM_HEAP_NODE)((QWORD)Memory - sizeof(MM_HEAP_NODE));

	if( node->Signature != MM_SIGNATURE ) return;

	node->Previous->Next = node->Next;
	if( node->Next != NULL ) node->Next->Previous = node->Previous;

	memset( node, 0x00, (node->Size + sizeof(MM_HEAP_NODE)) );
}
