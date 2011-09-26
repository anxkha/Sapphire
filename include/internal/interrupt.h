// ----------------------------------------------------------------------------
//  Filename: interrupt.h
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#define EXCEPTION_DE	0x00
#define EXCEPTION_BP	0x03
#define EXCEPTION_OF	0x04
#define EXCEPTION_BR	0x05
#define EXCEPTION_UD	0x06
#define EXCEPTION_NM	0x07
#define EXCEPTION_DF	0x08
#define EXCEPTION_TS	0x0A
#define EXCEPTION_NP	0x0B
#define EXCEPTION_SS	0x0C
#define EXCEPTION_GP	0x0D
#define EXCEPTION_PF	0x0E
#define EXCEPTION_MF	0x10
#define EXCEPTION_AC	0x11
#define EXCEPTION_MC	0x12
#define EXCEPTION_XF	0x13

typedef struct _KINTERRUPT_CONTEXT
{
	ULONG eax, ecx, edx, ebx, esi, edi, ebp;
	ULONG ds, es, fs, gs;
	ULONG num, error;
	ULONG eip, cs, eflags;
} KINTERRUPT_CONTEXT;

// idt.c
void			KeInitializeIDT();
void			KeSetIDTEntry( UCHAR Entry, ULONG Address, USHORT Selector, UCHAR Attributes );

// pic.c
void			KeInitializePIC();

void STDCALL	IoEnableIRQ( UCHAR Num );
void STDCALL	IoDisableIRQ( UCHAR Num );

BOOL STDCALL	IoRequestIRQ( UCHAR Num, void(*Handler)(KINTERRUPT_CONTEXT*) );
BOOL STDCALL	IoReleaseIRQ( UCHAR Num, void(*Handler)(KINTERRUPT_CONTEXT*) );

#endif // _INTERRUPT_H
