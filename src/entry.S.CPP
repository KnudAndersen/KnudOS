#define NO_INLINE
#include <memlayout.h>

bits 64

section .kernel_entry
extern kernel_main
kernel_entry:
    mov rsp, KSTACK_HI_NTH_NC(0) 
    mov rbp, KSTACK_HI_NTH_NC(0)
    mov rdi, rbx
    mov rsi, rcx
    mov rax, kernel_main
    call rax
err_loop:
    hlt
    jmp err_loop
    
