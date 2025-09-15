#ifndef KERNEL_C
#define KERNEL_C

#include "io.h"
#include "lib64_common.h"
#include "tty.h"
#include "vga.h"

tty_t tty0 = (tty_t){0};
void init_kernel() {
    /* TODO:
     * kstack, vmm/kheap, formatted io/logging
     */
    tty_init(&tty0, VGA_ROWS_MAX, VGA_COLS_MAX, TTY_DEFAULT_FG, TTY_DEFAULT_BG);
    vga_tty_render(&tty0);
}
void kernel_main(uint32_t stack_lo, uint32_t stack_hi) {
    init_kernel();
    kprints("hello world");
    while (1) {
        asm volatile("hlt");
    }
    return;
}

#endif
