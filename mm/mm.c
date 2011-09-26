// Preprocessor directives.
#include <sddk.h>
#include "mem.h"




/* Global declarations. */
ULONG		FreePageCount	= 0;
ULONG		NumberOfPages	= 0;
ULONG		HeapStart		= 0;
ULONG		HeapEnd			= 0;
ULONG		LowMemStart		= 0;
ULONG		LowMemEnd		= 0;
ULONG		SizeOfPageList	= 0;
ULONG		MemorySize		= 0;

MEMAREA*	PageList		= NULL;
MEMNODE*	pHeapBase		= NULL;
MEMNODE*	pLowMemBase		= NULL;




// ----------------------------------------------------------------------------
//  Name: MmInit
//
//  Desc: Initializes the base memory manager.
// ----------------------------------------------------------------------------
BOOL
MmInit( ULONG Size )
{
	ULONG	i;

	/* Verify that the size of memory is valid. */
	if( 0 == Size ) return FALSE;

	MemorySize = Size;

	/* Calculate the total number of pages. */
	FreePageCount = MemorySize / MM_PAGE_SIZE;
	NumberOfPages = FreePageCount;

	/* Calculate the size of the page list. */
	SizeOfPageList = NumberOfPages * sizeof(MEMAREA);

	/* Position the page list in a logical area. */
	PageList = (MEMAREA*)((MemorySize + 0) - SizeOfPageList);

	/* Zero out the page list. */
	memset( PageList, 0x00, SizeOfPageList );

	/* Lock the memory used for the page list. */
	for( i = (NumberOfPages - ROUND_UP(SizeOfPageList, MM_PAGE_SIZE)); i < NumberOfPages; i++ )
	{
		PageList[i].Allocated = 1;
		PageList[i].Size = 0;
	}

	return TRUE;
}




// ----------------------------------------------------------------------------
//  Name: MmInitPaging
//
//  Desc: Initializes the paging subsystem of the memory manager.
// ----------------------------------------------------------------------------
VOID
MmInitPaging()
{
}




// ----------------------------------------------------------------------------
//  Name: MmFindFirstFreePage
//
//  Desc: Finds the first free page that has a certain number of free pages
//        after it in the page list.
// ----------------------------------------------------------------------------
ULONG
MmFindFirstFreePage( ULONG PageCount )
{
	ULONG Available = 0;
	ULONG i = 0;

	for( i = (NumberOfPages - 1); i > 0; i-- )
	{
		if( PageList[i].Allocated == TRUE )
		{
			Available = 0;
			continue;
		}
		else
		{
			Available++;
		}

		if( Available >= PageCount )
		{
			break;
		}
	}

	return i;
}




// ----------------------------------------------------------------------------
//  Name: MmAllocatePageAlignedMemory
//
//  Desc: Allocates page aligned memory.
// ----------------------------------------------------------------------------
PVOID
MmAllocatePageAlignedMemory( ULONG Size )
{
	ULONG PageCount = 0;
	ULONG FreePage = 0;
	ULONG Address = 0;
	ULONG i;

	if( Size == 0 ) return NULL;

	PageCount = Size / MM_PAGE_SIZE;

	FreePage = MmFindFirstFreePage( PageCount );
	if( FreePage == 0 ) return NULL;

	Address = FreePage * MM_PAGE_SIZE;

	PageList[FreePage].Allocated = TRUE;
	PageList[FreePage].Size = PageCount;

	for( i = (FreePage + 1); i < (FreePage + PageCount); i++ )
	{
		PageList[i].Allocated = TRUE;
		PageList[i].Size = 0;
	}

	FreePageCount -= PageCount;

	return (PVOID)Address;
}




// ----------------------------------------------------------------------------
//  Name: MmFreePageAlignedMemory
//
//  Desc: Frees page aligned memory.
// ----------------------------------------------------------------------------
VOID
MmFreePageAlignedMemory( PVOID Memory )
{
	ULONG Page;
	ULONG i;

	if( Memory == NULL ) return;

	Page = (ULONG)Memory / MM_PAGE_SIZE;

	if( PageList[Page].Allocated != TRUE ) return;

	FreePageCount += PageList[Page].Size;

	for( i = Page; i < PageList[Page].Size; i++ )
	{
		PageList[i].Allocated = FALSE;
	}

	PageList[Page].Size = 0;
}

