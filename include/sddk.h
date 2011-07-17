#ifndef _SDDK_H
#define _SDDK_H

#include "types.h"
#include "status.h"

/* -------------------------- Thread Syncronization ------------------------ */

typedef struct PS_MUTEX, *PPS_MUTEX;

/* ps\mutex.c */

VOID
PsMutexAcquire( PPS_MUTEX pMutex );

VOID
PsMutexRelease( PPS_MUTEX pMutex );

STATUS
PsMutexCreate( PPS_MUTEX* ppMutex );

STATUS
PsMutexDestroy( PPS_MUTEX pMutex );

/* ------------------------------------------------------------------------- */

/* ---------------------------- Memory Management -------------------------- */

#define MM_PAGE_SIZE	0x1000

/* Memory allocation types */
#define MM_TYPE_KERNEL	1
#define MM_TYPE_DMA		2

/* mm\heap.c */

PVOID
MmHeapAllocate( QWORD Size,
			    BYTE Type );

VOID
MmHeapFree( PVOID pMemory,
		    BYTE Type );

/* mm\mm.c */

PVOID
MmAllocatePageAligned( DWORD NumPages,
					   BYTE Type );

VOID
MmFreePageAligned( PVOID pMemory,
				   BYTE Type );

/* ------------------------------------------------------------------------- */

/* --------------------------- Security components ------------------------- */

/* Object Types */
#define SE_OBJECT_TYPE_USER					1
#define SE_OBJECT_TYPE_COMPUTER				2
#define SE_OBJECT_TYPE_GROUP				3
#define SE_OBJECT_TYPE_ORGANIZATIONAL_UNIT	4

typedef struct _SID
{
	QWORD	ObjectId;
	BYTE	ObjectType;
} SID, *PSID;

typedef struct _SE_TOKEN
{
	PSID pSID;
} SE_TOKEN, *PSE_TOKEN;

/* ------------------------------------------------------------------------- */

/* ------------------------------ I/O Operations --------------------------- */

/* Device types */
#define IO_DEVICE_TYPE_INVALID	0
#define IO_DEVICE_TYPE_HID		1
#define IO_DEVICE_TYPE_DISPLAY	2
#define IO_DEVICE_TYPE_AUDIO	3
#define IO_DEVICE_TYPE_PORT		4

typedef struct _IO_DEVICE_CALLTABLE
{
} IO_DEVICE_CALLTABLE, *PIO_DEVICE_CALLTABLE;

typedef struct _IO_DEVICE
{
	BYTE				Type;
	PCHAR				Name;
	IO_DEVICE_CALLTABLE	Control;
} IO_DEVICE; *PIO_DEVICE;

/* io\device.c */

STATUS
IoRegisterDevice( PIO_DEVICE pDevice );

STATUS
IoUnregisterDevice( PIO_DEVICE pDevice );

STATUS
IoGetDeviceByName( PCHAR Name,
				   PIO_DEVICE* ppDevice );

/* ------------------------------------------------------------------------- */

/* ---------------------------- Virtual Filesystem ------------------------- */

typedef struct VFS_HANDLE, *PVFS_HANDLE;

typedef struct _VFS_CALLTABLE
{
	STATUS(*Create)( PVFS_HANDLE* ppHandle );
	STATUS(*Delete)( PVFS_HANDLE pHandle );

	STATUS(*Open)( PCHAR FilePath,
				   PCHAR FileName,
				   BYTE Mode,
				   PVFS_HANDLE* ppHandle );

	STATUS(*Close)( PVFS_HANDLE pHandle );
	STATUS(*Read)( PVFS_HANDLE pHandle );
	STATUS(*Write)( PVFS_HANDLE pHandle );
	STATUS(*Mount)( PIO_DEVICE pDevice, PVOID* ppData );
	STATUS(*Unmount)( PVOID pData );
	STATUS(*Seek)( PVFS_HANDLE pHandle );
	STATUS(*GetSeekPos)( PVFS_HANDLE pHandle );
	STATUS(*List)( PCHAR FilePath );
} VFS_CALLTABLE, *PVFS_CALLTABLE;

typedef struct _VFS_FILESYSTEM
{
	PCHAR			Name;
	VFS_CALLTABLE	Control;
} VFS_FILESYSTEM, *PVFS_FILESYSTEM;

/* vfs\vfs.c */

STATUS
VfsRegisterFilesystem( PVFS_FILESYSTEM pFilesystem );

STATUS
VfsUnregisterFilesystem( PVFS_FILESYSTEM pFilesystem );

/* vfs\mount.c */

STATUS
VfsMount( PCHAR MountpointName,
		  PCHAR FilesystemName, 
		  PCHAR DeviceName );

/* ------------------------------------------------------------------------- */

/* ----------------------------- List Operations --------------------------- */

#define LIST_CREATE( node_name, data_type ) \
	typedef struct _##node_name \
	{ \
		struct node_name* Previous; \
		struct data_type Data; \
		struct node_name* Next; \
	} node_name, *P##node_name;

/* ------------------------------------------------------------------------- */

#endif /* _SDDK_H */
