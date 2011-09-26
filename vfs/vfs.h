#ifndef _VFS_H
#define _VFS_H

struct _VFS_MOUNTPOINT
{
	PCHAR			MountName;
	VFS_FILESYSTEM*	Filesystem;
	IO_DEVICE*		Device;
	PVOID			Data;
};

struct _VFS_HANDLE
{
	VFS_MOUNTPOINT* pMountpoint;
	BYTE			Mode;
	PCHAR			FilePath;
	PCHAR			FileName;
	PVOID			Data;
};

/* vfs\mount.c */

extern PS_MUTEX* VfsMountpointListMutex;

STATUS
VfsGetMountpointByName( PCHAR Name,
						VFS_MOUNTPOINT** ppMountpoint );

/* vfs\vfs.c */

STATUS
VfsGetFilesystemByName( PCHAR Name,
						VFS_FILESYSTEM** ppFilesystem );

#endif /* _VFS_H */
