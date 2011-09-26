// ----------------------------------------------------------------------------
//  Filename: video.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// Global declarations.
VOID	scroll();

SHORT	ScreenX = 0;
SHORT	ScreenY = 0;

PUCHAR	vidmem;




// ----------------------------------------------------------------------------
//  Name: KeInitializeVideo
//
//  Desc: Initializes the display.
// ----------------------------------------------------------------------------
VOID KeInitializeVideo()
{
	ScreenX = 0;
	ScreenY = 0;

	vidmem = (PUCHAR)0xB8000;

	int i;

	for( i = 0; i < (25 * 80 + 80) * 2; i += 2 )
	{
		vidmem[i + 0] = ' ';
		vidmem[i + 1] = 0x07;
	}

	outb( 0x3D4, 14 );
	outb( 0x3D5, 0x00 );
	outb( 0x3D4, 15 );
	outb( 0x3D5, 0x00 );
}




// ----------------------------------------------------------------------------
//  Name: putch
//
//  Desc: Puts a character on the screen.
// ----------------------------------------------------------------------------
VOID putch( CHAR c )
{
	int i;

	switch( c )
	{
	case '\n':
		putch( '\r' );
		ScreenY++;

		break;

	case '\r':
		ScreenX = 0;

		break;

	case '\t':
		if( ScreenX % 8 )
		{
			for( i = 0; i < (ScreenX % 8); i++ )
			{
				putch( ' ' );
			}
		}
		else
		{
			for( i = 0; i < 8; i++ )
			{
				putch( ' ' );
			}
		}

		break;

	case '\b':
		ScreenX--;

		break;

	default:
		vidmem[(ScreenY * 80 + ScreenX) * 2 + 0] = c;
		vidmem[(ScreenY * 80 + ScreenX) * 2 + 1] = 0x07;

		ScreenX++;

		break;
	}

	if( ScreenX >= 80 )
	{
		ScreenX = 0;
		ScreenY++;
	}
	if( ScreenX < 0 )
	{
		ScreenX = 79;
		ScreenY--;
	}
	if( ScreenY >= 25 )
	{
		scroll();

		ScreenY = 24;
	}
	if( ScreenY < 0 )
	{
		ScreenX = 0;
		ScreenY = 0;
	}

	if( c == '\b' )
	{
		vidmem[(ScreenY * 80 + ScreenX) * 2 + 0] = ' ';
		vidmem[(ScreenY * 80 + ScreenX) * 2 + 1] = 0x07;
	}

	i = ScreenY * 80 + ScreenX;

	outb( 0x3D4, 14 );
	outb( 0x3D5, (UCHAR)(i >> 8) );
	outb( 0x3D4, 15 );
	outb( 0x3D5, (UCHAR)(i & 0xFF) );
}




// ----------------------------------------------------------------------------
//  Name: scroll
//
//  Desc: Scrolls the screen.
// ----------------------------------------------------------------------------
VOID scroll()
{
	int i;

	for( i = 1; i < 25; i++ )
	{
		memcpy( &vidmem[((i - 1) * 80) * 2], &vidmem[(i * 80) * 2], 80 * 2 );
	}

	memset( &vidmem[(24 * 80) * 2], 0x00, 80 * 2 );
}
