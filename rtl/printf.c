// ----------------------------------------------------------------------------
//  Filename: printf.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// ----------------------------------------------------------------------------
//  Name: printf
//
//  Desc: Prints formatted text to the screen.
// ----------------------------------------------------------------------------
VOID printf( PCHAR format, ... )
{
	char c;
	PINT data = (PINT)(PVOID)&format;
	PCHAR str;
	CHAR ptr[16];

	data++;

	while( (c = *(format++)) != '\0' )
	{
		if( c == '%' )
		{
			switch( c = *(format++) )
			{
			case '%':
				putch( c );
				break;

			case 'c':
				putch( (*(data++) & 0xFF) );
				break;

			case 'd':
			case 'u':
			case 'x':
				*ConvertToAscii( ptr, c, *((PULONG)data++) ) = 0;

				str = ptr;

				while( *str )
				{
					putch( *(str++) );
				}

				break;

			case 's':
				str = (PCHAR)(*(data++));

				while( *str != '\0' )
				{
					putch( *(str++) );
				}

				break;

			case '\0':
				return;

			default:
				printf( "Invalid format specifier: %%%c\n", c );
				return;
			}
		}
		else
		{
			putch( c );
		}
	}
}
