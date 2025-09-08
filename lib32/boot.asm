bits 32
MB_PG_ALIGN equ (1 << 0)
MB_MMAP equ (1 << 1)
MB_MAGIC equ 0x1BADB002
MB_FLAGS equ (MB_PG_ALIGN + MB_MMAP)
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
dd MB_MAGIC
dd MB_FLAGS
dd MB_CHECKSUM

STACK_SIZE equ (16 * 1024)
section .bss
alignb 16
boot_stack_lo:
    resb STACK_SIZE
boot_stack_hi:

LOADER_ERR equ (0)
section .text
global start
start:
    mov esp, boot_stack_hi
    mov ebp, boot_stack_hi
    push boot_stack_hi
    extern loader_main
    call loader_main
    cmp eax, LOADER_ERR
    je err_loop
    ; edi=arg1, esi=arg2
    jmp far [eax]
err_loop:
    cli
    hlt
    jmp err_loop
