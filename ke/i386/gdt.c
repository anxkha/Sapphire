// ----------------------------------------------------------------------------
//  Filename: gdt.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>




// Global declarations.
USHORT KernelGDT[6 * 4] = 
{
	0x0000, 0x0000, 0x0000, 0x0000,
	0xFFFF, 0x0000, 0x9A00, 0x00CF,
	0xFFFF, 0x0000, 0x9200, 0x00CF,
	0xFFFF, 0x0000, 0xFA00, 0x00CF,
	0xFFFF, 0x0000, 0xF200, 0x00CF,
	0x0000, 0x0000, 0x0000, 0x0000
};




// ----------------------------------------------------------------------------
//  Name: KeSetGDTEntry
//
//  Desc: Sets up an entry in the kernel GDT.
// ----------------------------------------------------------------------------
VOID KeSetGDTEntry( UCHAR Entry, ULONG Base, ULONG Limit, UCHAR Access, UCHAR Attributes )
{
	KernelGDT[(Entry / 2) + 0] = (USHORT)(Limit & 0xFFFF);
	KernelGDT[(Entry / 2) + 1] = (USHORT)(Base & 0xFFFF);
	KernelGDT[(Entry / 2) + 2] = (USHORT)(((Base >> 16) & 0xFF) | (Access << 8));
	KernelGDT[(Entry / 2) + 3] = (USHORT)(((Limit >> 16) & 0x0F) | (Attributes << 4) | (Base >> 24));
}
