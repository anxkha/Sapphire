#ifndef _MM_H
#define _MM_H

#define MM_SIGNATURE		0xDEADBEEF

typedef struct _MM_HEAP_NODE
{
	DWORD Signature;
	QWORD Size;

	struct _MEMNODE* Next;
	struct _MEMNODE* Previous;
} MM_HEAP_NODE, *PMM_HEAP_NODE;

typedef struct _MEMAREA
{
	ULONG Allocated;
	ULONG Size;
} MEMAREA;

typedef struct _MEMNODE
{
	struct _MEMNODE* Previous;
	ULONG Size;
	struct _MEMNODE* Next;
} MEMNODE;

extern ULONG	FreePageCount;
extern ULONG	NumberOfPages;
extern ULONG	HeapStart;
extern ULONG	HeapEnd;
extern ULONG	LowMemStart;
extern ULONG	LowMemEnd;
extern ULONG	UserHeapStart;
extern ULONG	UserHeapEnd;

extern MEMAREA*	PageList;
extern MEMNODE*	pHeapBase;
extern MEMNODE*	pLowMemBase;
extern MEMNODE*	pUserHeapBase;

#endif /* _MM_H */
