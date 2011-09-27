// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// Global declarations.
MEMNODE*	HeapBase;
ULONG		HeapEnd;




// ----------------------------------------------------------------------------
//  Name: mm_init_heap
//
//  Desc: Initializes the kernel memory heap.
// ----------------------------------------------------------------------------
VOID
mm_init_heap( ULONG MemSize )
{
	HeapBase = (MEMNODE*)MmAllocatePageAlignedMemory( MemSize );
	HeapBase->Signature = MEM_SIGNATURE;
	HeapBase->Size = 0;
	HeapBase->Next = NULL;
	HeapBase->Previous = NULL;

	HeapEnd = (ULONG)HeapBase + MemSize;
}




// ----------------------------------------------------------------------------
//  Name: mm_heap_alloc
//
//  Desc: Allocates memory on a heap.
// ----------------------------------------------------------------------------
PVOID
mm_heap_alloc( ULONG Size,
			   BYTE Type )
{
	MEMNODE* pNew;
	MEMNODE* pCurrent;
	PVOID Address = NULL;

	pCurrent = HeapBase;

	while( pCurrent != NULL )
	{
		if( pCurrent->Next == NULL )
		{
			if( ((ULONG)pCurrent + (2 * sizeof(MEMNODE)) + pCurrent->Size + Size) >= HeapEnd ) return NULL;

			pNew = (MEMNODE*)((ULONG)pCurrent + sizeof(MEMNODE) + pCurrent->Size);
			pNew->Signature = MEM_SIGNATURE;
			pNew->Size = Size;
			pNew->Next = NULL;
			pNew->Previous = pCurrent;

			pCurrent->Next = pNew;

			Address = (PVOID)((ULONG)pNew + sizeof(MEMNODE));

			break;
		}

		if( ((ULONG)pCurrent + (2 * sizeof(MEMNODE)) + pCurrent->Size + Size) < (ULONG)pCurrent->Next )
		{
			pNew = (MEMNODE*)((ULONG)pCurrent + sizeof(MEMNODE) + pCurrent->Size);
			pNew->Signature = MEM_SIGNATURE;
			pNew->Size = Size;
			pNew->Next = pCurrent->Next;
			pNew->Previous = pCurrent;

			pCurrent->Next->Previous = pNew;
			pCurrent->Next = pNew;

			Address = (PVOID)(pNew + sizeof(MEMNODE));

			break;
		}

		pCurrent = pCurrent->Next;
	}

	return Address;
}




// ----------------------------------------------------------------------------
//  Name: mm_heap_free
//
//  Desc: Frees memory from a heap.
// ----------------------------------------------------------------------------
VOID
mm_heap_free( PVOID Memory,
			  BYTE Type )
{
	MEMNODE* node;

	if( (ULONG)Memory < ((ULONG)HeapBase + sizeof(MEMNODE)) ) return;

	node = (MEMNODE*)((ULONG)Memory - sizeof(MEMNODE));

	if( node->Signature != MEM_SIGNATURE ) return;

	node->Previous->Next = node->Next;
	if( node->Next != NULL ) node->Next->Previous = node->Previous;

	memset( node, 0x00, (node->Size + sizeof(MEMNODE)) );
}
