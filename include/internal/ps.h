#ifndef _PS_H
#define _PS_H

typedef struct _KTSS
{
	ULONG BackLink;
	ULONG esp0;
	USHORT ss0, reserved0;
	ULONG esp1;
	USHORT ss1, reserved1;
	ULONG esp2;
	USHORT ss2, reserved2;
	ULONG cr3, eip, eflags;
	ULONG eax, ecx, edx, ebx;
	ULONG esp, ebp, esi, edi;
	USHORT es, reserved3;
	USHORT cs, reserved4;
	USHORT ss, reserved5;
	USHORT ds, reserved6;
	USHORT fs, reserved7;
	USHORT gs, reserved8;
	USHORT ldt, reserved9;
	USHORT debug, iobitmap;
} KTSS;

#endif /* _PS_H */
