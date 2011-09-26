// ----------------------------------------------------------------------------
//  Filename: stdlib.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// ----------------------------------------------------------------------------
//  Name: ConvertToAscii
//
//  Desc: Converts a number to it's ascii counterpart.
// ----------------------------------------------------------------------------
PCHAR ConvertToAscii( PCHAR Buffer, INT c, ... )
{
	ULONG num = *((&c) + 1), mult = 10;
	char *ptr = Buffer;

	if (c == 'x')
		mult = 16;

	if( (num & 0x80000000uL) && (c == 'd') )
	{
		num = (~num) + 1;
		*(ptr++) = '-';
		Buffer++;
	}

	do
	{
		int dig = num % mult;
		*(ptr++) = ( (dig > 9) ? dig + 'a' - 10 : '0' + dig );
	} while( num /= mult );

	// Reorder to correct direction!
	{
		PCHAR ptr1 = ptr - 1;
		PCHAR ptr2 = Buffer;

		while( ptr1 > ptr2 )
		{
			int c = *ptr1;

			*ptr1 = *ptr2;
			*ptr2 = c;
			ptr1--;
			ptr2++;
		}
	}

	return ptr;
}




// ----------------------------------------------------------------------------
//  Name: memset
//
//  Desc: Sets memory in an area to a certain value.
// ----------------------------------------------------------------------------
void* memset( void* Dest, int Value, unsigned int Size )
{
	unsigned char* D = (unsigned char*)Dest;

	while( Size-- > 0 ) *(D++) = Value;

	return Dest;
}




// ----------------------------------------------------------------------------
//  Name: memcpy
//
//  Desc: Copies memory from one location to another.
// ----------------------------------------------------------------------------
void* memcpy( void* Dest, const void* Src, unsigned int Size )
{
	unsigned char* D = (unsigned char*)Dest;
	unsigned char* S = (unsigned char*)Src;

	while( Size-- > 0 ) *(D++) = *(S++);

	return Dest;
}




// ----------------------------------------------------------------------------
//  Name: memcmp
//
//  Desc: Compares two areas of memory.
// ----------------------------------------------------------------------------
int memcmp( const void* Dest, const void* Src, unsigned int Size )
{
	unsigned char* D = (unsigned char*)Dest;
	unsigned char* S = (unsigned char*)Src;

	while( Size-- > 0 )
	{
		if( *S != *D ) return (*S - *D);

		S++;
		D++;
	}

	return 0;
}




// ----------------------------------------------------------------------------
//  Name: strlen
//
//  Desc: Finds the length of a string.
// ----------------------------------------------------------------------------
int strlen( const char* String )
{
	const char* end = String;

	while( *end++ );

	return (int)(end - String - 1);
}




// ----------------------------------------------------------------------------
//  Name: strcpy
//
//  Desc: Copies one string into another.
// ----------------------------------------------------------------------------
char* strcpy( char* Dest, const char* Src )
{
	char c;

	while( (c = *(Src++)) != '\0' ) *(Dest++) = c;

	*Dest = '\0';

	return Dest;
}




// ----------------------------------------------------------------------------
//  Name: strncpy
//
//  Desc: Copies at most n characters from one string into another.
// ----------------------------------------------------------------------------
char* strncpy( char* Dest, const char* Src, int Count )
{
	char* start = Dest;

	while( Count && (*Dest++ = *Src++) ) Count--;

	if( Count )
	{
		while( --Count ) *Dest++ = '\0';
	}

	return start;
}




// ----------------------------------------------------------------------------
//  Name: strcmp
//
//  Desc: Compares two strings.
// ----------------------------------------------------------------------------
int strcmp( const char* Dest, const char* Src )
{
	int ret = 0;

	while( !(ret = *(unsigned char*)Src - *(unsigned char*)Dest) && *Dest ) ++Src, ++Dest;

	if( ret < 0 )
		ret = -1;
	else if( ret > 0 )
		ret = 1;

	return ret;
}
