#include <sddk.h>

VOID
KeInit()
{
	KeInitializeTSS();
	KeInitializeIDT();

	MmInit();
	MmInitPaging();

	KeInitializePIC();

	ASSERT( STATUS_SUCCESS == IoInitDeviceManager() );
	ASSERT( STATUS_SUCCESS == VfsInit() );

	for( ; ; );
}
