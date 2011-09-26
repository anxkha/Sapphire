// ----------------------------------------------------------------------------
//  Filename: idt.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// Global declarations.
typedef struct _IDTEntry
{
	USHORT AddressL;
	USHORT Selector;
	UCHAR Unused;
	UCHAR Attributes;
	USHORT AddressH;
} PACKED IDTEntry;

VOID	KeHandleInterrupt( KINTERRUPT_CONTEXT* ctx );

IDTEntry		KernelIDT[256];
extern ULONG	KeInterruptList[20];




// ----------------------------------------------------------------------------
//  Name: KeInitializeIDT
//
//  Desc: Initializes the kernel IDT.
// ----------------------------------------------------------------------------
VOID KeInitializeIDT()
{
	int i;

	for( i = 0; i < 20; i++ )
	{
		KeSetIDTEntry( i, KeInterruptList[i], KERNEL_CS, 0x8E );
	}
}




// ----------------------------------------------------------------------------
//  Name: KeSetIDTEntry
//
//  Desc: Sets an entry in the kernel IDT.
// ----------------------------------------------------------------------------
VOID KeSetIDTEntry( UCHAR Entry, ULONG Address, USHORT Selector, UCHAR Attributes )
{
	KernelIDT[Entry].AddressL = (USHORT)(Address & 0xFFFF);
	KernelIDT[Entry].Selector = Selector;
	KernelIDT[Entry].Unused = 0;
	KernelIDT[Entry].Attributes = Attributes;
	KernelIDT[Entry].AddressH = (USHORT)(Address >> 16);
}




// ----------------------------------------------------------------------------
//  Name: KeHandleInterrupt
//
//  Desc: Handles an exception.
// ----------------------------------------------------------------------------
VOID KeHandleInterrupt( KINTERRUPT_CONTEXT* ctx )
{
	ULONG cr3;

	cli;

	get_cr3( cr3 );

	switch( ctx->num )
	{
	case EXCEPTION_DE:
		printf( "Divide Error\n" );
		break;

	case EXCEPTION_BP:
		printf( "Breakpoint\n" );
		break;

	case EXCEPTION_OF:
		printf( "Overflow\n" );
		break;

	case EXCEPTION_BR:
		printf( "BOUND Range Exceeded\n" );
		break;

	case EXCEPTION_UD:
		printf( "Invalid Opcode (Undefined Opcode)\n" );
		break;

	case EXCEPTION_NM:
		printf( "Device Not Available (No Math Coprocessor)\n" );
		break;

	case EXCEPTION_DF:
		printf( "Double Fault\n" );
		break;

	case EXCEPTION_TS:
		printf( "Invalid TSS\n" );
		break;

	case EXCEPTION_NP:
		printf( "Segment Not Present\n" );
		break;

	case EXCEPTION_SS:
		printf( "Stack-Segment Fault\n" );
		break;

	case EXCEPTION_GP:
		printf( "General Protection Fault\n" );
		break;

	case EXCEPTION_PF:
		printf( "Page Fault\n" );
		break;

	case EXCEPTION_MF:
		printf( "x87 FPU Floating-Point Error (Math Fault)\n" );
		break;

	case EXCEPTION_AC:
		printf( "Alignment Check\n" );
		break;

	case EXCEPTION_MC:
		printf( "Machine Check\n" );
		break;

	case EXCEPTION_XF:
		printf( "SIMD Floating-Point Exception\n" );
		break;

	default:
		printf( "Reserved exception\n" );
		break;
	}

	printf( "Exception Number: %d\n\n", ctx->num );
	printf( "Register Dump:\n" );
	printf( "EAX = 0x%x\tECX = 0x%x\tEDX = 0x%x\tEBX = 0x%x\n", ctx->eax, ctx->ecx, ctx->edx, ctx->ebx );
	printf( "ESI = 0x%x\tEDI = 0x%x\tEBP = 0x%x\n", ctx->esi, ctx->edi, ctx->ebp );
	printf( "DS = 0x%x\tES = 0x%x\tFS = 0x%x\tGS = 0x%x\n", ctx->ds, ctx->es, ctx->fs, ctx->gs );
	printf( "CS:EIP = 0x%x:0x%x\tEFLAGS = 0x%x\n", ctx->cs, ctx->eip, ctx->eflags );
	printf( "CR3 = 0x%x\n\n", cr3 );
	printf( "Halting system..." );

	for( ; ; );
}
