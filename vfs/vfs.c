#include <sddk.h>
#include "vfs.h"

LIST_CREATE( VFS_FILESYSTEM_NODE, VFS_FILESYSTEM )

VFS_FILESYSTEM_NODE*	VfsFilesystemListHead	= NULL;
VFS_FILESYSTEM_NODE*	VfsFilesystemListTail	= NULL;
PS_MUTEX*				VfsFilesystemListMutex	= NULL;

STATUS
VfsInit()
{
	STATUS result = STATUS_SUCCESS;

	result = PsMutexCreate( &VfsFilesystemListMutex );
	if( !SUCCESS( result ) ) goto done;

	result = PsMutexCreate( &VfsMountpointListMutex );
	if( !SUCCESS( result ) ) goto done;

done:
	if( VfsFilesystemListMutex ) PsMutexDestroy( VfsFilesystemListMutex );
	if( VfsMountpointListMutex ) PsMutexDestroy( VfsMountpointListMutex );

	return result;
}

STATUS
VfsRegisterFilesystem( VFS_FILESYSTEM* pFilesystem )
{
	STATUS result = STATUS_SUCCESS;
	VFS_FILESYSTEM_NODE* node = NULL;
	VFS_FILESYSTEM* pTempFS = NULL;

	/* Basic sanity checks. */
	if( !pFilesystem )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	if( !pFilesystem->Name )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	if( !strlen( pFilesystem->Name ) )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	/* Make sure another filesystem hasn't registered under the same name. */
	if( STATUS_SUCCESS == VfsGetFilesystemByName( pFilesystem->Name, &pTempFS ) )
	{
		result = STATUS_ALREADY_EXISTS;
		goto done;
	}

	node = (VFS_FILESYSTEM_NODE*)MmHeapAllocate( sizeof(VFS_FILESYSTEM_NODE), MM_TYPE_KERNEL );
	if( !node )
	{
		result = STATUS_OUT_OF_MEMORY;
		goto done;
	}

	node->Previous = NULL;
	node->Data = pFilesystem;
	node->Next = NULL;

	PsMutexAcquire( VfsFilesystemListMutex );

	/* Insert the new filesystem into the list. */
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

	PsMutexRelease( VfsFilesystemListMutex );

done:
	if( !SUCCESS(result) )
	{
		if( node ) MmHeapFree( node, MM_TYPE_KERNEL );
	}

	return result;
}

STATUS
VfsUnregisterFilesystem( VFS_FILESYSTEM* pFilesystem )
{
	STATUS result = STATUS_SUCCESS;
	VFS_FILESYSTEM_NODE* node = NULL;

	PsMutexAcquire( VfsFilesystemListMutex );

	/* Basic sanity checks. */
	if( !pFilesystem )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	node = VfsFilesystemListHead;
	while( node )
	{
		if( node->Data == pFilesystem )
		{
			/* We found it! */

			/* TODO: Figure out if this filesystem is mounted, and fail if it
			   is. */

			/* Remove it from the list and destroy it. */
			if( VfsFilesystemListHead == VfsFilesystemListTail )
			{
				VfsFilesystemListHead = NULL;
				VfsFilesystemListTail = NULL;
			}
			else
			{
				if( node == VfsFilesystemListHead )
				{
					VfsFilesystemListHead = node->Next;
					VfsFilesystemListHead->Previous = NULL;
				}
				else if( node == VfsFilesystemListTail )
				{
					VfsFilesystemListTail = node->Previous;
					VfsFilesystemListTail->Next = NULL;
				}
				else
				{
					node->Previous->Next = node->Next;
					node->Next->Previous = node->Previous;
				}
			}

			MmHeapFree( node, MM_TYPE_KERNEL );

			goto done;
		}

		node = node->Next;
	}

	/* We didn't find it. */
	result = STATUS_NOT_FOUND;

done:
	PsMutexRelease( VfsFilesystemListMutex );

	return result;
}

STATUS
VfsGetFilesystemByName( PCHAR Name,
						VFS_FILESYSTEM** ppFilesystem )
{
	STATUS result = STATUS_SUCCESS;
	VFS_FILESYSTEM_NODE* node = NULL;

	/* This is an internal function. It is never meant to, nor should it ever
	   be, a public facing function. */

	/* Basic sanity checks. */
	if( !Name ) return STATUS_INVALID_PARAMETER;
	if( !strlen( Name ) ) return STATUS_INVALID_PARAMETER;

	PsMutexAcquire( VfsFilesystemListMutex );

	if( !ppFilesystem )
	{
		result = STATUS_INVALID_PARAMETER;
		goto done;
	}

	node = VfsFilesystemListHead;

	while( node )
	{
		if( !strcmp( node->Data->Name, Name ) )
		{
			*ppFilesystem = node->Data;
			goto done;
		}

		node = node->Next;
	}

	result = STATUS_NOT_FOUND;
	*ppFilesystem = NULL;

done:
	PsMutexRelease( VfsFilesystemListMutex );

	return result;
}
