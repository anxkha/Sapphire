// ----------------------------------------------------------------------------
//  Filename: version.c
//  Author: Lucas Suggs
// ----------------------------------------------------------------------------




// Preprocessor directives.
#include <sddk.h>
#include <internal\kernel.h>




// ----------------------------------------------------------------------------
//  Name: RtlDisplayKernelVersion
//
//  Desc: Displays the kernel version number on the screen.
// ----------------------------------------------------------------------------
VOID RtlDisplayKernelVersion()
{
	ULONG major, minor, build;

	major = KERNEL_VERSION_MAJOR;
	minor = KERNEL_VERSION_MINOR;
	build = KERNEL_VERSION_BUILD;

	// Print out the kernel version.
	printf( "Sapphire kernel, version %u.%u.%u\n\n", major, minor, build );
}
