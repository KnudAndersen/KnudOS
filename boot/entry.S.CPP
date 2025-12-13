#define NO_INLINE
#include <params.h>
#include <memlayout.h>
bits 32

section .multiboot
align 4
dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

global boot_stack_hi
section .bss
boot_stack_lo:
resb KSTACK_SIZE_NC
boot_stack_hi:

section .data
multiboot_info:
dd 0

section .text

; PROPOGATED TO KERNEL
; EBX, ECX, EDX => SYSV X86-64 CC src/entry.S.CPP
extern loader_main
; extern __linker_loader_end

global boot_entry
boot_entry:

    mov [multiboot_info], ebx
    cmp eax, MULTIBOOT_RESPONSE
    jne err_inf

    mov esp, boot_stack_hi
    mov ebp, boot_stack_hi

    push ebx
    call loader_main

    mov ebx, [multiboot_info]
    ; mov ecx, __linker_loader_end

    jmp far [eax]

err_inf:
    cli
    hlt
    jmp err_inf
