// ----------------------------------------------------------------------------
//  Filename: stdlib.h
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------
#ifndef _STDLIB_H
#define _STDLIB_H

void*	memset( void* Dest, int Value, unsigned int Size );
void*	memcpy( void* Dest, const void* Src, unsigned int Size );
int		memcmp( const void* Dest, const void* Src, unsigned int Size );

int		strlen( const char* String );
int		strcmp( const char* Dest, const char* Src );
char*	strcpy( char* Dest, const char* Src );
char*	strncpy( char* Dest, const char* Src, int Count );

PCHAR	ConvertToAscii( PCHAR Buffer, INT c, ... );

#endif // _STDLIB_H
