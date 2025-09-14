bits 64


section .cba
extern kernel_main
global jump_to_kernel
jump_to_kernel:
    mov rax, kernel_main
    jmp rax

    
