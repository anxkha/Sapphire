// ----------------------------------------------------------------------------
//  Filename: tss.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>




// Global declarations.
KTSS	KernelTSS;




// ----------------------------------------------------------------------------
//  Name: KeInitializeTSS
//
//  Desc: Initializes the system TSS.
// ----------------------------------------------------------------------------
VOID KeInitializeTSS()
{
	KernelTSS.esp0 = 0x7C000;
	KernelTSS.ss0 = KERNEL_DS;
	KernelTSS.iobitmap = 0xFFFF;
	KernelTSS.ldt = 0;

	KeSetGDTEntry( KERNEL_TSS, (ULONG)&KernelTSS, (sizeof(KernelTSS) - 1), 0x89, 0x00 );
}
