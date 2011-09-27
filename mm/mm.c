// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




/* Global declarations. */
ULONG		FreePageCount	= 0;
ULONG		NumberOfPages	= 0;
ULONG		HeapStart		= 0;
ULONG		HeapEnd			= 0;
ULONG		LowMemStart		= 0;
ULONG		LowMemEnd		= 0;
ULONG		SizeOfPageList	= 0;
ULONG		MemorySize		= 0;
ULONG		MemoryStart		= 0;
ULONG		MemoryEnd		= 0;

MEMAREA*	PageList		= NULL;
MEMNODE*	pHeapBase		= NULL;
MEMNODE*	pLowMemBase		= NULL;




// ----------------------------------------------------------------------------
//  Name: mm_init
//
//  Desc: Initializes the base memory manager.
// ----------------------------------------------------------------------------
BOOL
mm_init( ULONG Size )
{
	ULONG	i;

	/* Verify that the size of memory is valid. */
	if( 0 == Size ) return FALSE;

	MemorySize = Size;
	MemoryStart = 0x100000;
	MemoryEnd = MemoryStart + MemorySize;

	/* Calculate the total number of pages. */
	FreePageCount = MemorySize / MM_PAGE_SIZE;
	NumberOfPages = FreePageCount;

	/* Calculate the size of the page list. */
	SizeOfPageList = NumberOfPages * sizeof(MEMAREA);

	/* Position the page list in a logical area. */
	PageList = (MEMAREA*)((MemoryEnd + 0) - SizeOfPageList);

	/* Zero out the page list. */
	memset( PageList, 0x00, SizeOfPageList );

	/* Lock the memory used for the page list. */
	for( i = (NumberOfPages - ROUND_UP(SizeOfPageList, MM_PAGE_SIZE)); i < NumberOfPages; i++ )
	{
		PageList[i].Allocated = 1;
		PageList[i].Size = 0;
	}

	FreePageCount = FreePageCount - ROUND_UP(SizeOfPageList, MM_PAGE_SIZE); 

	return TRUE;
}




// ----------------------------------------------------------------------------
//  Name: mm_init_paging
//
//  Desc: Initializes the paging subsystem of the memory manager.
// ----------------------------------------------------------------------------
VOID
mm_init_paging()
{
}




// ----------------------------------------------------------------------------
//  Name: mm_find_free_page
//
//  Desc: Finds the first free page that has a certain number of free pages
//        after it in the page list.
// ----------------------------------------------------------------------------
ULONG
mm_find_free_page( ULONG PageCount )
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

	PageCount = ROUND_UP(Size, MM_PAGE_SIZE) / MM_PAGE_SIZE;

	FreePage = mm_find_free_page( PageCount );
	if( FreePage == 0 ) return NULL;

	Address = MemoryStart + (FreePage * MM_PAGE_SIZE);

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

	Page = (ULONG)(Memory - MemoryStart) / MM_PAGE_SIZE;

	if( PageList[Page].Allocated != TRUE ) return;

	FreePageCount += PageList[Page].Size;

	for( i = Page; i < PageList[Page].Size; i++ )
	{
		PageList[i].Allocated = FALSE;
	}

	PageList[Page].Size = 0;
}




// ----------------------------------------------------------------------------
//  Name: mm_reserve_region
//
//  Desc: Reserves a specific region of memory.
// ----------------------------------------------------------------------------
PVOID
mm_reserve_region( PVOID RegionStart,
				   ULONG RegionSize )
{
	ULONG Available;
	ULONG i, j, StartPage, NumPages;

	Available = 1;
	StartPage = ((ULONG)RegionStart - MemoryStart) / MM_PAGE_SIZE;
	NumPages = ROUND_UP(RegionSize, MM_PAGE_SIZE) / MM_PAGE_SIZE;
	j = NumPages + StartPage;

	for( i = StartPage; i < j; i += 1 )
	{
		if( TRUE == PageList[i].Allocated )
		{
			Available = 0;
			break;
		}
	}

	if( 0 == Available ) return NULL;

	PageList[StartPage].Allocated = TRUE;
	PageList[StartPage].Size = NumPages;

	for( i = (StartPage + 1); i < j; i += 1 )
	{
		PageList[i].Allocated = TRUE;
		PageList[i].Size = 0;
	}

	FreePageCount = FreePageCount - NumPages;

	return RegionStart;
}
