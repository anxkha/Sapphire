#include <sddk.h>

/* ----------------------------------------------------------------------------
 Name: outb

 Desc: Writes a byte to a port.
---------------------------------------------------------------------------- */
VOID
outb( USHORT Port, UCHAR Value )
{
	__asm( "outb %0, %w1\n\t" : : "a" (Value), "d" (Port) );
	/*__asm
	{
		mov edx, Port;
		mov al, Value;
		out dx, al;
	};*/
}




/* ----------------------------------------------------------------------------
 Name: outw

 Desc: Writes a word to a port.
---------------------------------------------------------------------------- */
VOID
outw( USHORT Port, USHORT Value )
{
	__asm( "outw %0, %w1\n\t" : : "a" (Value), "d" (Port) );
	/*__asm
	{
		mov edx, Port;
		mov ax, Value;
		out dx, ax;
	};*/
}




/* ----------------------------------------------------------------------------
 Name: outl

 Desc: Writes a dword to a port.
---------------------------------------------------------------------------- */
VOID
outl( USHORT Port, ULONG Value )
{
	__asm( "outl %0, %w1\n\t" : : "a" (Value), "d" (Port) );
	/*__asm
	{
		mov edx, Port;
		mov eax, Value;
		out dx, eax;
	};*/
}




/* ----------------------------------------------------------------------------
 Name: inb

 Desc: Reads a byte from a port.
---------------------------------------------------------------------------- */
UCHAR
inb( USHORT Port )
{
	UCHAR Value;

	__asm( "inb %w1, %0\n\t" : "=a" (Value) : "d" (Port) );

	/*__asm
	{
		mov edx, Port;
		in al, dx
		mov Value, al;
	};*/

	return Value;
}




/* ----------------------------------------------------------------------------
 Name: inw

 Desc: Reads a word from a port.
---------------------------------------------------------------------------- */
USHORT
inw( USHORT Port )
{
	USHORT Value;

	__asm( "inw %w1, %0\n\t" : "=a" (Value) : "d" (Port) );

	/*__asm
	{
		mov edx, Port;
		in ax, dx
		mov Value, ax;
	};*/

	return Value;
}



/* ----------------------------------------------------------------------------
 Name: inl

 Desc: Reads a dword from a port.
---------------------------------------------------------------------------- */
ULONG
inl( USHORT Port )
{
	ULONG Value;

	__asm( "inl %w1, %0\n\t" : "=a" (Value) : "d" (Port) );

	/*__asm
	{
		mov edx, Port;
		in eax, dx
		mov Value, eax;
	};*/

	return Value;
}
