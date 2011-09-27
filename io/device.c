#include <sddk.h>

LIST_CREATE( IO_DEVICE_NODE, IO_DEVICE )

IO_DEVICE_NODE*	IoDeviceListHead	= NULL;
IO_DEVICE_NODE*	IoDeviceListTail	= NULL;
PS_MUTEX*		IoDeviceListMutex	= NULL;




// ----------------------------------------------------------------------------
//  Name: IoInitDeviceManager
//
//  Desc: Initializes the device manager.
// ----------------------------------------------------------------------------
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




// ----------------------------------------------------------------------------
//  Name: IoRegisterDevice
//
//  Desc: Register a new Device.
// ----------------------------------------------------------------------------
STATUS
IoRegisterDevice( IO_DEVICE* pDevice )
{
	STATUS result = STATUS_SUCCESS;
	IO_DEVICE_NODE* node = NULL;

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

	node = (IO_DEVICE_NODE*)kmalloc( sizeof(IO_DEVICE_NODE), MM_TYPE_KERNEL );
	if( !node )
	{
		result = STATUS_OUT_OF_MEMORY;
		goto done;
	}

	node->Previous = NULL;
	node->Data = pDevice;
	node->Next = NULL;

	PsMutexAcquire( IoDeviceListMutex );

	if( !IoDeviceListHead )
	{
		IoDeviceListHead = node;
		IoDeviceListTail = node;
	}
	else
	{
		node->Previous = IoDeviceListTail;
		node->Previous->Next = node;
		IoDeviceListTail = node;
	}

	PsMutexRelease( IoDeviceListMutex );

done:
	if( !SUCCESS(result) )
	{
		if( node ) kfree( node, MM_TYPE_KERNEL );
	}

	return result;
}




// ----------------------------------------------------------------------------
//  Name: IoUnregisterDevice
//
//  Desc: Unregisters a device given its pointer.
// ----------------------------------------------------------------------------
STATUS
IoUnregisterDevice( IO_DEVICE* pDevice )
{
	STATUS result = STATUS_SUCCESS;
	IO_DEVICE_NODE* node = NULL;

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

			kfree( node, MM_TYPE_KERNEL );

			goto done;
		}

		node = node->Next;
	}

	result = STATUS_NOT_FOUND;

done:
	PsMutexRelease( IoDeviceListMutex );

	return result;
}




// ----------------------------------------------------------------------------
//  Name: IoGetDeviceByName
//
//  Desc: Searches for a Device by its name and returns it if found.
// ----------------------------------------------------------------------------
STATUS
IoGetDeviceByName( PCHAR Name,
				   IO_DEVICE** ppDevice )
{
	STATUS result = STATUS_SUCCESS;

	// TODO: Implement this?

	return result;
}
