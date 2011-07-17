#include <sddk.h>

VOID
KeInit()
{
	MmInit();
	MmInitPaging();

	ASSERT( STATUS_SUCCESS == IoInitDeviceManager() );
	ASSERT( STATUS_SUCCESS == VfsInit() );

	for( ; ; );
}
