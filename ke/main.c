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

	RtlDisplayKernelVersion();

	ASSERT( mm_init( pMultibootHeader->mem_upper ) );
	mm_init_paging();
	mm_init_heap( 0x400000 );

	//PsInitializeThreadManager();

	//KeInitializePIC();

	ASSERT( STATUS_SUCCESS == IoInitDeviceManager() );
	ASSERT( STATUS_SUCCESS == VfsInit() );

	printf( "waka waka\n" );

	for( ; ; );
}
