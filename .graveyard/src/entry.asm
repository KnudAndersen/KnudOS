bits 64


section .kernel_entry
extern kernel_main
extern __KSTACK_TOP_VADDR__
global jump_to_kernel
jump_to_kernel:
    mov rax, __KSTACK_TOP_VADDR__
    mov rsp, [rax]
    mov rbp, rsp
    mov rax, kernel_main
    call rax
    jmp err_loop
err_loop:
    cli
    hlt
    jmp err_loop

    
