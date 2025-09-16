#ifndef KERNEL_C
#define KERNEL_C

#include "./lib/include/io.h"
#include "./lib/include/kcommon.h"
#include "./lib/include/tty.h"
#include "./lib/include/vga.h"

tty_t tty0 = (tty_t){0};
tty_t tty1 = (tty_t){0};
void init_kernel() {
    /* TODO:
     * map kstack
     * map ACPI region and other MMIO like APIC
     * pass down multiboot info from boostrap
     * initialize pmm accounting for all in-use pages (use multiboot mmap)
     * set up paging again using the pmm and HHDM
     * vmm/kheap
     * formatted io/logging
     */
    tty_init(&tty0, VGA_ROWS_MAX, VGA_COLS_MAX / 2, TTY_DEFAULT_FG, TTY_DEFAULT_BG, 0, 0);
    tty_init(&tty1, VGA_ROWS_MAX, VGA_COLS_MAX / 2, TTY_DEFAULT_FG, TTY_DEFAULT_BG,
             VGA_COLS_MAX / 2, 0);
    vga_tty_render(&tty0);
    vga_tty_render(&tty1);
}
void kernel_main(uint32_t stack_lo, uint32_t stack_hi) {
    init_kernel();
    uint32_t i = 0;
    kprints("hello world\n");
    kprints("hello world\n");
    tprints(&tty1, "this tty1");
    while (1) {
        asm volatile("hlt");
    }
    return;
}
#endif
