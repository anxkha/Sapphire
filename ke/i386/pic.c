// ----------------------------------------------------------------------------
//  Filename: pic.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// Global declarations.
VOID(*KeIRQHandlers[16])(KINTERRUPT_CONTEXT*);

extern	ULONG	KeIRQList[16];




// ----------------------------------------------------------------------------
//  Name: KeInitializePIC
//
//  Desc: Initializes the system PIC.
// ----------------------------------------------------------------------------
VOID KeInitializePIC()
{
	int i;

	outb( 0x20, 0x11 );
	outb( 0xA0, 0x11 );
	outb( 0x21, 0x20 );
	outb( 0xA1, 0x28 );
	outb( 0x21, 0x04 );
	outb( 0xA1, 0x02 );
	outb( 0x21, 0x01 );
	outb( 0xA1, 0x01 );
	outb( 0x21, 0xFF );
	outb( 0xA1, 0xFF );

	for( i = 0; i < 16; i++ )
	{
		KeIRQHandlers[i] = NULL;
		KeSetIDTEntry( i + 0x20, KeIRQList[i], KERNEL_CS, 0x8E );
	}

	IoEnableIRQ( 2 );

	sti;
}




// ----------------------------------------------------------------------------
//  Exported
//
//  Name: IoEnableIRQ
//
//  Desc: Enables an IRQ on the PICs.
// ----------------------------------------------------------------------------
VOID STDCALL IoEnableIRQ( UCHAR Num )
{
	ULONG flags;

	get_flags( flags );
	cli;

	if( Num > 7 )
	{
		outb( 0xA1, inb( 0xA1 ) & ~(1 << (Num - 8)) );
	}
	else
	{
		outb( 0x21, inb( 0x21 ) & ~(1 << Num) );
	}

	set_flags( flags );
}




// ----------------------------------------------------------------------------
//  Exported
//
//  Name: IoDisableIRQ
//
//  Desc: Disables an IRQ on the PICs.
// ----------------------------------------------------------------------------
VOID STDCALL IoDisableIRQ( UCHAR Num )
{
	ULONG flags;

	get_flags( flags );
	cli;

	if( Num > 7 )
	{
		outb( 0xA1, inb( 0xA1 ) | (1 << (Num - 8)) );
	}
	else
	{
		outb( 0x21, inb( 0x21 ) | (1 << Num) );
	}

	set_flags( flags );
}




// ----------------------------------------------------------------------------
//  Exported
//
//  Name: IoRequestIRQ
//
//  Desc: Requests the usage of an IRQ and registers the handler with
//        the system.
// ----------------------------------------------------------------------------
BOOL STDCALL IoRequestIRQ( UCHAR Num, VOID(*Handler)(KINTERRUPT_CONTEXT*) )
{
	ULONG flags;

	if( Num > 15 ) return FALSE;
	if( KeIRQHandlers[Num] != NULL ) return FALSE;
	if( Handler == NULL ) return FALSE;

	get_flags( flags );
	cli;

	KeIRQHandlers[Num] = Handler;

	set_flags( flags );

	return TRUE;
}




// ----------------------------------------------------------------------------
//  Exported
//
//  Name: IoReleaseIRQ
//
//  Desc: Releases the usage of an IRQ and unregisters the handler from
//        the system.
// ----------------------------------------------------------------------------
BOOL STDCALL IoReleaseIRQ( UCHAR Num, VOID(*Handler)(KINTERRUPT_CONTEXT*) )
{
	ULONG flags;

	if( Num > 15 ) return FALSE;
	if( Handler == NULL ) return FALSE;
	if( KeIRQHandlers[Num] != Handler ) return FALSE;

	get_flags( flags );
	cli;

	KeIRQHandlers[Num] = NULL;

	set_flags( flags );

	return TRUE;
}




// ----------------------------------------------------------------------------
//  Name: KeHandleIRQ
//
//  Desc: Handles a hardware interrupt.
// ----------------------------------------------------------------------------
VOID KeHandleIRQ( KINTERRUPT_CONTEXT* ctx )
{
	cli;

	outb( 0x20, 0x20 );
	if( ctx->num > 7 ) outb( 0xA0, 0x20 );

	if( KeIRQHandlers[ctx->num] != NULL ) KeIRQHandlers[ctx->num]( ctx );

	sti;
}
