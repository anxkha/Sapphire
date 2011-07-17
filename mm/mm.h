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

#endif /* _MM_H */
