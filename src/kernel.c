#ifndef KERNEL_C
#define KERNEL_C

#include "./lib/include/io.h"
#include "./lib/include/kcommon.h"
#include "./lib/include/memory.h"
#include "./lib/include/tty.h"
#include "./lib/include/vga.h"

tty_t tty0 = (tty_t){0};
tty_t ttys[3] = {0};
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
    tty_init(&ttys[0], VGA_ROWS_MAX, VGA_COLS_MAX / 2, TTY_DEFAULT_FG, TTY_DEFAULT_BG,
             VGA_COLS_MAX / 2, 0);

    vga_tty_render(&tty0);
    vga_tty_render(&ttys[0]);
}

void kernel_main() {
    /* ************************************ */
    /* TREAT AS KNUD OS ABI, DO NOT MODIFY! */
    uint32_t __multiboot = pop_reg32(ESI);
    uint32_t __stack_top = pop_reg32(EDI);
    /* ************************************ */
    multiboot_info* m_info = (multiboot_info*)(uint64_t)__multiboot;
    init_kernel();
    pmm_init(m_info);
    // char test[15 * 1024 + 1024 - 431]; // 431 - 432
    while (1) {
        asm volatile("hlt");
    }
    return;
}
#endif
