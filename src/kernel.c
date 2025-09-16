#ifndef KERNEL_C
#define KERNEL_C

#include "./lib/include/io.h"
#include "./lib/include/kcommon.h"
#include "./lib/include/memory.h"
#include "./lib/include/tty.h"
#include "./lib/include/vga.h"

tty_t tty0 = (tty_t){0};
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
    vga_tty_render(&tty0);
}
void kernel_main(uint32_t multiboot, uint32_t stack_hi) {
    init_kernel();
    pmm_init((multiboot_info*)(uintptr_t)multiboot);
    while (1) {
        asm volatile("hlt");
    }
    return;
}
#endif
