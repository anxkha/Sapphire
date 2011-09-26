#ifndef _TYPES_H
#define _TYPES_H

typedef unsigned char BYTE, UCHAR, U8, BOOL, *PUCHAR, *PBYTE;
typedef unsigned short WORD, USHORT, U16, *PUSHORT, *PWORD;
typedef unsigned int UINT, *PUINT;
typedef unsigned long DWORD, ULONG, U32, *PULONG, *PDWORD;
typedef unsigned long long QWORD, ULONGLONG, U64, *PULONGLONG, *PQWORD;

typedef char CHAR, S8, *PCHAR;
typedef short SHORT, S16, *PSHORT;
typedef int INT, *PINT;
typedef long LONG, S32, *PLONG;
typedef long long LONGLONG, S64, *PLONGLONG;

typedef void VOID, *PVOID;

#define NULL 0
#define TRUE 1
#define FALSE 0

#define PACKED		__attribute__((packed))
#ifndef __stdcall
#define __stdcall	__attribute__((stdcall))
#endif // __stdcall
#define STDCALL __stdcall

#define ROUND_UP(N, S)		((N) + (S) - ((N) % (S)))
#define ROUND_DOWN(N, S)	((N) - ((N) % (S)))

#define LOWORD(x)	(x & 0xFFFF)
#define HIWORD(x)	(x >> 16)
#define LOBYTE(w)	(w & 0xFF)
#define HIBYTE(w)	((WORD)w >> 8)

#endif /* _TYPES_H */
