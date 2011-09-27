#ifndef _KERNEL_H
#define _KERNEL_H

#include "interrupt.h"
#include "ps.h"
#include "multiboot.h"
#include "mm.h"

#define KERNEL_VERSION_MAJOR	0
#define KERNEL_VERSION_MINOR	0
#define KERNEL_VERSION_BUILD	1

#define KERNEL_CS	0x08
#define KERNEL_DS	0x10
#define USER_CS		0x18
#define USER_DS		0x20
#define KERNEL_TSS	0x28

#define cli				__asm( "cli\n\t" )
#define sti				__asm( "sti\n\t" )
#define get_flags(x)	__asm( "pushfl \n\t popl %0\n\t" : "=g" (x) );
#define set_flags(x)	__asm( "pushl %0 \n\t popfl\n\t" : : "g" (x) );
#define get_cr3(x)		__asm( "movl %%cr3, %0\n\t" : "=d" (x) );
#define set_cr3(x)		__asm( "movl %0, %%cr3\n\t" : : "r" (x) );
#define get_cr2(x)		__asm( "movl %%cr2, %0\n\t" : "=d" (x) );

#endif /* _KERNEL_H */
