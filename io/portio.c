#include <sddk.h>

/* ----------------------------------------------------------------------------
 Name: WRITE_PORT_UCHAR

 Desc: Writes a byte to a port.
---------------------------------------------------------------------------- */
VOID
WRITE_PORT_UCHAR( PUCHAR Port, UCHAR Value )
{
	__asm
	{
		mov edx, Port;
		mov al, Value;
		out dx, al;
	};
}




/* ----------------------------------------------------------------------------
 Name: WRITE_PORT_USHORT

 Desc: Writes a word to a port.
---------------------------------------------------------------------------- */
VOID
WRITE_PORT_USHORT( PUSHORT Port, USHORT Value )
{
	__asm
	{
		mov edx, Port;
		mov ax, Value;
		out dx, ax;
	};
}




/* ----------------------------------------------------------------------------
 Name: WRITE_PORT_ULONG

 Desc: Writes a dword to a port.
---------------------------------------------------------------------------- */
VOID
WRITE_PORT_ULONG( PULONG Port, ULONG Value )
{
	__asm
	{
		mov edx, Port;
		mov eax, Value;
		out dx, eax;
	};
}




/* ----------------------------------------------------------------------------
 Name: READ_PORT_UCHAR

 Desc: Reads a byte from a port.
---------------------------------------------------------------------------- */
UCHAR
READ_PORT_UCHAR( PUCHAR Port )
{
	UCHAR Value;

	__asm
	{
		mov edx, Port;
		in al, dx
		mov Value, al;
	};

	return Value;
}




/* ----------------------------------------------------------------------------
 Name: READ_PORT_USHORT

 Desc: Reads a word from a port.
---------------------------------------------------------------------------- */
USHORT
READ_PORT_USHORT( PUSHORT Port )
{
	USHORT Value;

	__asm
	{
		mov edx, Port;
		in ax, dx
		mov Value, ax;
	};

	return Value;
}



/* ----------------------------------------------------------------------------
 Name: READ_PORT_ULONG

 Desc: Reads a dword from a port.
---------------------------------------------------------------------------- */
ULONG
READ_PORT_ULONG( PULONG Port )
{
	ULONG Value;

	__asm
	{
		mov edx, Port;
		in eax, dx
		mov Value, eax;
	};

	return Value;
}
