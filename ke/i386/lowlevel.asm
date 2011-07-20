[BITS 32]
[SECTION .text]
[GLOBAL _KeInterruptList]
[GLOBAL _KeIRQList]
[EXTERN _KeHandleInterrupt]
[EXTERN _KeHandleIRQ]

HandleINT:
	push gs
	push fs
	push es
	push ds
	push ebp
	push edi
	push esi
	push ebx
	push edx
	push ecx
	push eax
	
	push esp
	call _KeHandleInterrupt
	add esp, 4
	
	pop eax
	pop ecx
	pop edx
	pop ebx
	pop esi
	pop edi
	pop ebp
	pop ds
	pop es
	pop fs
	pop gs
	
	add esp, 8
	
	iret

HandleIRQ:
	push gs
	push fs
	push es
	push ds
	push ebp
	push edi
	push esi
	push ebx
	push edx
	push ecx
	push eax
	
	push esp
	call _KeHandleIRQ
	add esp, 4
	
	pop eax
	pop ecx
	pop edx
	pop ebx
	pop esi
	pop edi
	pop ebp
	pop ds
	pop es
	pop fs
	pop gs
	
	add esp, 8
	
	iret

%macro INTR 1
INTR_%1:
	push dword 0
	push dword %1
	jmp HandleINT
%endmacro

%macro INTR_ERR 1
INTR_%1:
	push dword %1
	jmp HandleINT
%endmacro

%macro IRQ 1
IRQ_%1:
	push dword 0
	push dword %1
	jmp HandleIRQ
%endmacro

INTR 0
INTR 1
INTR 2
INTR 3
INTR 4
INTR 5
INTR 6
INTR 7
INTR_ERR 8
INTR 9
INTR_ERR 10
INTR_ERR 11
INTR_ERR 12
INTR_ERR 13
INTR_ERR 14
INTR 15
INTR 16
INTR_ERR 17
INTR 18
INTR 19

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

_KeInterruptList:
	dd INTR_0
	dd INTR_1
	dd INTR_2
	dd INTR_3
	dd INTR_4
	dd INTR_5
	dd INTR_6
	dd INTR_7
	dd INTR_8
	dd INTR_9
	dd INTR_10
	dd INTR_11
	dd INTR_12
	dd INTR_13
	dd INTR_14
	dd INTR_15
	dd INTR_16
	dd INTR_17
	dd INTR_18
	dd INTR_19

_KeIRQList:
	dd IRQ_0
	dd IRQ_1
	dd IRQ_2
	dd IRQ_3
	dd IRQ_4
	dd IRQ_5
	dd IRQ_6
	dd IRQ_7
	dd IRQ_8
	dd IRQ_9
	dd IRQ_10
	dd IRQ_11
	dd IRQ_12
	dd IRQ_13
	dd IRQ_14
	dd IRQ_15
