#ifndef _VFS_H
#define _VFS_H

typedef struct _VFS_MOUNTPOINT
{
	PCHAR			MountName;
	PVFS_FILESYSTEM	Filesystem;
	PIO_DEVICE		Device;
	PVOID			Data;
} VFS_MOUNTPOINT, *PVFS_MOUNTPOINT;

typedef struct _VFS_HANDLE
{
	PVFS_MOUNTPOINT pMountpoint;
	BYTE			Mode;
	PCHAR			FilePath;
	PCHAR			FileName;
	PVOID			Data;
} VFS_HANDLE, *PVFS_HANDLE;

/* vfs\mount.c */

extern PPS_MUTEX VfsMountpointListMutex;

STATUS
VfsGetMountpointByName( PCHAR Name,
						PVFS_MOUNTPOINT* ppMountpoint );

/* vfs\vfs.c */

STATUS
VfsGetFilesystemByName( PCHAR Name,
						PVFS_FILESYSTEM* ppFilesystem );

#endif /* _VFS_H */
