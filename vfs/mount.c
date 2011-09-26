#include <sddk.h>
#include "vfs.h"

LIST_CREATE( VFS_MOUNTPOINT_NODE, VFS_MOUNTPOINT )

VFS_MOUNTPOINT_NODE*	VfsMountpointListHead	= NULL;
VFS_MOUNTPOINT_NODE*	VfsMountpointListTail	= NULL;
PS_MUTEX*				VfsMountpointListMutex	= NULL;

STATUS
VfsMount( PCHAR MountpointName,
		  PCHAR FilesystemName, 
		  PCHAR DeviceName )
{
	STATUS result = STATUS_SUCCESS;
	VFS_FILESYSTEM* pFilesystem = NULL;
	IO_DEVICE* pDevice = NULL;
	VFS_MOUNTPOINT* pMountpoint = NULL;
	VFS_MOUNTPOINT_NODE* node = NULL;

	/* Basic sanity checks. */
	if( !MountpointName ) result = STATUS_INVALID_PARAMETER;
	if( !strlen( MountpointName ) ) result = STATUS_INVALID_PARAMETER;
	if( !FilesystemName ) result = STATUS_INVALID_PARAMETER;
	if( !strlen( FilesystemName ) ) result = STATUS_INVALID_PARAMETER;
	if( !DeviceName ) result = STATUS_INVALID_PARAMETER;
	if( !strlen( DeviceName ) ) result = STATUS_INVALID_PARAMETER;
	if( !SUCCESS(result) ) goto done;

	result = VfsGetMountpointByName( MountpointName, &pMountpoint );
	if( SUCCESS(result) )
	{
		result = STATUS_ALREADY_EXISTS;
		goto done;
	}

	/* Attempt to find the referenced filesystem. */
	result = VfsGetFilesystemByName( FilesystemName, &pFilesystem );
	if( !SUCCESS(result) ) goto done;

	/* Attempt to find the reference device. */
	result = IoGetDeviceByName( DeviceName,	&pDevice );
	if( !SUCCESS(result) ) goto done;

	/* Allocate data structures. */
	node = (VFS_MOUNTPOINT_NODE*)MmHeapAllocate( sizeof(VFS_MOUNTPOINT_NODE), MM_TYPE_KERNEL );
	if( !node )
	{
		result = STATUS_OUT_OF_MEMORY;
		goto done;
	}

	pMountpoint = (VFS_MOUNTPOINT*)MmHeapAllocate( sizeof(VFS_MOUNTPOINT), MM_TYPE_KERNEL );
	if( !pMountpoint )
	{
		result = STATUS_OUT_OF_MEMORY;
		goto done;
	}

	pMountpoint->MountName = (PCHAR)MmHeapAllocate( (strlen(MountpointName) + 1), MM_TYPE_KERNEL );
	if( !pMountpoint->MountName )
	{
		result = STATUS_OUT_OF_MEMORY;
		goto done;
	}

	/* Copy necessary information. */
	strcpy( MountpointName, pMountpoint->MountName );
	pMountpoint->Filesystem = pFilesystem;
	pMountpoint->Device = pDevice;

	/* Set up the list node. */
	node->Previous = NULL;
	node->Data = pMountpoint;
	node->Next = NULL;

	/* Attempt to actually mount the device using the filesystem. This will
	   fail if the device is improper or does not contain the referenced
	   filesystem. */
	result = pFilesystem->Control.Mount( pDevice, &pMountpoint->Data );
	if( !SUCCESS(result) ) goto done;

	PsMutexAcquire( VfsMountpointListMutex );

	if( !VfsMountpointListHead )
	{
		VfsMountpointListHead = node;
		VfsMountpointListTail = node;
	}
	else
	{
		node->Previous = VfsMountpointListTail;
		node->Previous->Next = node;
		VfsMountpointListTail = node;
	}

	PsMutexRelease( VfsMountpointListMutex );

done:
	if( !SUCCESS(result) )
	{
		if( pMountpoint )
		{
			if( pMountpoint->MountName ) MmHeapFree( pMountpoint->MountName, MM_TYPE_KERNEL );
			MmHeapFree( pMountpoint, MM_TYPE_KERNEL );
		}

		if( node ) MmHeapFree( node, MM_TYPE_KERNEL );
	}

	return result;
}

STATUS
VfsGetMountpointByName( PCHAR Name,
						VFS_MOUNTPOINT** ppMountpoint )
{
	STATUS result = STATUS_SUCCESS;
	VFS_MOUNTPOINT_NODE* node = NULL;

	/* This is an internal function. It is never meant to, nor should it ever
	   be, a public facing function. */

	/* Basic sanity checks. */
	if( !Name ) return NULL;
	if( !strlen( Name ) ) return NULL;

	PsMutexAcquire( VfsMountpointListMutex );

	if( !ppMountpoint )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	node = VfsMountpointListHead;

	while( node )
	{
		if( !strcmp( node->Data->MountName, Name ) )
		{
			*ppMountpoint = node->Data;
			goto done;
		}

		node = node->Next;
	}

	result = STATUS_NOT_FOUND;
	*ppMountpoint = NULL;

done:
	PsMutexRelease( VfsMountpointListMutex );

	return result;
}
