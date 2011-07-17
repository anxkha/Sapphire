#include <sddk.h>

LIST_CREATE( IO_DEVICE_NODE, PIO_DEVICE )

PIO_DEVICE_NODE	IoDeviceListHead	= NULL;
PIO_DEVICE_NODE	IoDeviceListTail	= NULL;
PPS_MUTEX		IoDeviceListMutex	= NULL;

STATUS
IoInitDeviceManager()
{
	STATUS result = STATUS_SUCCESS;

	result = PsMutexCreate( &IoDeviceListMutex );
	if( !SUCCESS(result) ) goto done;

done:
	if( IoDeviceListMutex ) PsMutexDestroy( IoDeviceListMutex );

	return result;
}

STATUS
IoRegisterDevice( PIO_DEVICE pDevice )
{
	STATUS result = STATUS_SUCCESS;
	PIO_DEVICE_NODE node = NULL;

	if( !pDevice )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	if( !pDevice->Name )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	if( !strlen( pDevice->Name ) )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	node = MmAllocateHeap( sizeof(IO_DEVICE_NODE), MM_TYPE_KERNEL );
	if( !node )
	{
		result = STATUS_OUT_OF_MEMORY;
		goto done;
	}

	node->Previous = NULL;
	node->Data = pDevice;
	node->Next = NULL;

	PsMutexAcquire( IoDeviceListMutex );

	if( !VfsFilesystemListHead )
	{
		VfsFilesystemListHead = node;
		VfsFilesystemListTail = node;
	}
	else
	{
		node->Previous = VfsFilesystemListTail;
		node->Previous->Next = node;
		VfsFilesystemListTail = node;
	}

	PsMutexRelease( IoDeviceListMutex );

done:
	if( !SUCCESS(result) )
	{
		if( node ) MmFreeHeap( node, MM_TYPE_KERNEL );
	}

	return result;
}

STATUS
IoUnregisterDevice( PIO_DEVICE pDevice )
{
	STATUS result = STATUS_SUCCESS;
	PIO_DEVICE_NODE node = NULL;

	PsMutexAcquire( IoDeviceListMutex );

	node = IoDeviceListHead;

	while( node )
	{
		if( node->Data == pDevice )
		{
			if( IoDeviceListHead == IoDeviceListTail )
			{
				IoDeviceListHead = NULL;
				IoDeviceListTail = NULL;
			}
			else
			{
				if( node == IoDeviceListHead )
				{
					node->Next->Previous = NULL;
					IoDeviceListHead = node->Next;
				}
				else if( node == IoDeviceListTail )
				{
					node->Previous->Next = NULL;
					IoDeviceListTail = node->Previous;
				}
				else
				{
					node->Previous->Next = node->Next;
					node->Next->Previous = node->Previous;
				}
			}

			MmFreeHeap( node, MM_TYPE_KERNEL );

			goto done;
		}

		node = node->Next;
	}

	result = STATUS_NOT_FOUND;

done:
	PsMutexRelease( IoDeviceListMutex );

	return result;
}

STATUS
IoGetDeviceByName( PCHAR Name,
				   PIO_DEVICE* ppDevice )
{
	STATUS result = STATUS_SUCCESS;

	return result;
}
