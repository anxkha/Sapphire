// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// ----------------------------------------------------------------------------
//  Name: KeInit
//
//  Desc: 
// ----------------------------------------------------------------------------
VOID
KeInit( multiboot_info_t* pMultibootHeader )
{
	KeInitializeTSS();
	KeInitializeIDT();

	KeInitializeVideo();

	ASSERT( MmInit( pMultibootHeader->mem_upper ) );
	MmInitPaging();
	MmInitializeHeap();

	//PsInitializeThreadManager();

	//KeInitializePIC();

	ASSERT( STATUS_SUCCESS == IoInitDeviceManager() );
	ASSERT( STATUS_SUCCESS == VfsInit() );

	for( ; ; );
}
