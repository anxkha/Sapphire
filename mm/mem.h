// ----------------------------------------------------------------------------
//  Filename: mem.h
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------
#ifndef _MEM_H
#define _MEM_H

#define MEM_SIGNATURE		0xCEAFBEAF

#define PDE_VALID			0x001
#define PDE_WRITE			0x002
#define PDE_OWNER			0x004
#define PDE_WRITETHROUGH	0x008
#define PDE_CACHEDISABLE	0x010
#define PDE_ACCESSED		0x020
#define PDE_DIRTY			0x040
#define PDE_LARGEPAGE		0x080
#define PDE_GLOBAL			0x100
#define PDE_COPYONWRITE		0x200
#define PDE_PROTOTYPE		0x400

typedef struct MEMAREA
{
	BOOL Allocated;
	ULONG Size;
} PACKED MEMAREA;

typedef struct _MEMNODE
{
	ULONG Signature;
	ULONG Size;

	struct _MEMNODE* Next;
	struct _MEMNODE* Previous;
} PACKED MEMNODE;

// meminit.c
ULONG	MmFindFirstFreePage( ULONG PageCount );

extern MEMAREA*		PageList;
extern ULONG		FreePageCount;
extern ULONG		MemorySize;
extern ULONG		NumberOfPages;
extern ULONG		SizeOfPageList;

// kmalloc.c
VOID	MmInitializeHeap( ULONG MemSize );

extern MEMNODE*	HeapBase;
extern ULONG	HeapEnd;

#endif // _MEM_H
