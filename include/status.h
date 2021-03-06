#ifndef _STATUS_H
#define _STATUS_H

typedef DWORD STATUS;

#define SUCCESS(x) (STATUS_SUCCESS == x)

#define STATUS_SUCCESS	0

#define STATUS_OUT_OF_MEMORY		0x10000000
#define STATUS_ALREADY_EXISTS		0x10000010
#define STATUS_INVALID_PARAMETER	0x10000020
#define STATUS_NOT_FOUND			0x10000030
#define STATUS_IN_USE				0x10000040

#endif /* _STATUS_H */
