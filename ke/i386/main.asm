[BITS 32]
[GLOBAL _start]
[EXTERN _code]
[EXTERN _bss]
[EXTERN _end]
[EXTERN _KeInit]
[EXTERN _KernelGDT]
[EXTERN _KernelIDT]
[EXTERN _KeInitializeTSS]
[EXTERN _printf]
[EXTERN _RtlDisplayKernelVersion]

db 'B', 'L', 'U', 'E'
dd 0x100000
dd _start - 0x100000
dw 0
dw 2
dd 0
dd ExportDirectory - 0x100000

align 4

_start:
	mov esp, 0x7C000
	jmp boot

align 4
mboot:
	MULTIBOOT_PAGE_ALIGN	equ 1 << 0
	MULTIBOOT_MEMORY_INFO	equ 1 << 1
	MULTIBOOT_AOUT_KLUDGE	equ 1 << 16
	MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
	MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
	MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
	
	dd mboot
	dd _code
	dd _bss
	dd _end
	dd _start

boot:
	cli
	
	lgdt [GDTR]
	lidt [IDTR]
	
	jmp KERNEL_CS:NewGDT
	
NewGDT:
	mov ax, KERNEL_DS
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	push dword 0
	popf
	
	call _KeInitializeTSS
	
	mov ax, KERNEL_TSS
	ltr ax

	push ebx
	
	call _KeInit
	
	cli
	jmp $

KERNEL_CS equ 0x08
KERNEL_DS equ 0x10
KERNEL_TSS equ 0x28

[SECTION .data]

ExportDirectory:
	dd Export1Name - 0x100000
	dd _printf - 0x100000
	
	dd Export2Name - 0x100000
	dd _RtlDisplayKernelVersion - 0x100000

Export1Name dd 'printf', 0
Export2Name dd 'RtlDisplayKernelVersion', 0

GDTR:
	dw (6 * 8) - 1
	dd _KernelGDT

IDTR:
	dw 0x7FF
	dd _KernelIDT
