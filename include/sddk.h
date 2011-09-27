#ifndef _SDDK_H
#define _SDDK_H

#include "types.h"
#include "status.h"
#include "stdlib.h"

#define ASSERT(x) x

/* -------------------------- Thread Syncronization ------------------------ */

typedef struct _PS_MUTEX PS_MUTEX;

/* ps\mutex.c */

VOID
PsMutexAcquire( PS_MUTEX* pMutex );

VOID
PsMutexRelease( PS_MUTEX* pMutex );

STATUS
PsMutexCreate( PS_MUTEX** ppMutex );

STATUS
PsMutexDestroy( PS_MUTEX* pMutex );

/* ------------------------------------------------------------------------- */

/* ---------------------------- Memory Management -------------------------- */

#define MM_PAGE_SIZE	0x1000

/* Memory allocation types */
#define MM_TYPE_KERNEL	1
#define MM_TYPE_DMA		2
#define MM_TYPE_USER	3

/* mm\heap.c */

PVOID
mm_heap_alloc( ULONG Size,
			   BYTE Type );

VOID
mm_heap_free( PVOID Memory,
			  BYTE Type );

#define kmalloc(x, y) mm_heap_alloc( x, y )
#define kfree(x, y) mm_heap_free( x, y )

/* mm\mm.c */

PVOID
MmAllocatePageAligned( DWORD NumPages,
					   BYTE Type );

VOID
MmFreePageAligned( PVOID pMemory,
				   BYTE Type );

PVOID
mm_reserve_region( PVOID RegionStart,
				   ULONG RegionSize );

#define mm_release_region(x) MmFreePageAligned( x, MM_TYPE_KERNEL );

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
} IO_DEVICE_CALLTABLE;

typedef struct _IO_DEVICE
{
	BYTE				Type;
	PCHAR				Name;
	IO_DEVICE_CALLTABLE	Control;
} IO_DEVICE;

/* io\device.c */

STATUS
IoRegisterDevice( IO_DEVICE* pDevice );

STATUS
IoUnregisterDevice( IO_DEVICE* pDevice );

STATUS
IoGetDeviceByName( PCHAR Name,
				   IO_DEVICE** ppDevice );

/* io\portio.c */

VOID
outb( USHORT Port, UCHAR Value );

VOID
outw( USHORT Port, USHORT Value );

VOID
outl( USHORT Port, ULONG Value );

UCHAR
inb( USHORT Port );

USHORT
inw( USHORT Port );

ULONG
inl( USHORT Port );

/* ------------------------------------------------------------------------- */

/* ---------------------------- Virtual Filesystem ------------------------- */

typedef struct _VFS_MOUNTPOINT VFS_MOUNTPOINT;
typedef struct _VFS_HANDLE VFS_HANDLE;

typedef struct _VFS_CALLTABLE
{
	STATUS(*Create)( VFS_HANDLE** ppHandle );
	STATUS(*Delete)( VFS_HANDLE* pHandle );

	STATUS(*Open)( PCHAR FilePath,
				   PCHAR FileName,
				   BYTE Mode,
				   VFS_HANDLE** ppHandle );

	STATUS(*Close)( VFS_HANDLE* pHandle );
	STATUS(*Read)( VFS_HANDLE* pHandle );
	STATUS(*Write)( VFS_HANDLE* pHandle );
	STATUS(*Mount)( IO_DEVICE* pDevice, PVOID* ppData );
	STATUS(*Unmount)( PVOID pData );
	STATUS(*Seek)( VFS_HANDLE* pHandle );
	STATUS(*GetSeekPos)( VFS_HANDLE* pHandle );
	STATUS(*List)( PCHAR FilePath );
} VFS_CALLTABLE, *PVFS_CALLTABLE;

typedef struct _VFS_FILESYSTEM
{
	PCHAR			Name;
	VFS_CALLTABLE	Control;
} VFS_FILESYSTEM;

/* vfs\vfs.c */

STATUS
VfsRegisterFilesystem( VFS_FILESYSTEM* pFilesystem );

STATUS
VfsUnregisterFilesystem( VFS_FILESYSTEM* pFilesystem );

/* vfs\mount.c */

STATUS
VfsMount( PCHAR MountpointName,
		  PCHAR FilesystemName, 
		  PCHAR DeviceName );

/* ------------------------------------------------------------------------- */

/* ----------------------------- List Operations --------------------------- */

#define LIST_CREATE( node_name, data_type ) \
	typedef struct _##node_name node_name; \
	struct _##node_name \
	{ \
		node_name* Previous; \
		data_type* Data; \
		node_name* Next; \
	};

/* ------------------------------------------------------------------------- */

#endif /* _SDDK_H */
