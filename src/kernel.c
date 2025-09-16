#ifndef KERNEL_C
#define KERNEL_C

#include "./lib/include/io.h"
#include "./lib/include/kcommon.h"
#include "./lib/include/tty.h"
#include "./lib/include/vga.h"

tty_t tty0 = (tty_t){0};
tty_t ttys[4] = {0};
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
    tty_init(&tty0, VGA_ROWS_MAX / 2, VGA_COLS_MAX / 2, TTY_DEFAULT_FG, TTY_DEFAULT_BG, 0, 0);
    tty_init(&ttys[0], VGA_ROWS_MAX / 2, VGA_COLS_MAX / 2, TTY_DEFAULT_FG, TTY_DEFAULT_BG,
             VGA_COLS_MAX / 2, VGA_ROWS_MAX / 2);
    tty_init(&ttys[1], VGA_ROWS_MAX / 2, VGA_COLS_MAX / 2, TTY_DEFAULT_FG, TTY_DEFAULT_BG, 0,
             VGA_ROWS_MAX / 2);
    tty_init(&ttys[2], VGA_ROWS_MAX / 2, VGA_COLS_MAX / 2, TTY_DEFAULT_FG, TTY_DEFAULT_BG,
             VGA_COLS_MAX / 2, 0);

    vga_tty_render(&tty0);
    vga_tty_render(&ttys[0]);
    vga_tty_render(&ttys[1]);
    vga_tty_render(&ttys[2]);
}
void kernel_main(uint32_t stack_lo, uint32_t stack_hi) {
    init_kernel();
    // kprints("hello world");
    char c;
    uint8_t fg[4] = {0, 1, 2, 3};
    uint8_t bg[4] = {VGA_COLOR_BLACK, 5, 6, 7};
    uint32_t p[7] = {11, 29, 3, 7, 19, 13, 2};
    uint32_t i = 0, j = 0, k = 0;
    while (1) {
        c = 'A' + (j % ('Z' - 'A' + 1));
        kprintc(c);
        for (i = 0; i <= 2; ++i) {
            tprintc(&ttys[i], c);
        }
        vga_tty_render(&tty0);
        vga_tty_render(&ttys[0]);
        vga_tty_render(&ttys[1]);
        vga_tty_render(&ttys[2]);
        for (i = 0; i <= 3; ++i) {
            fg[i] = (fg[i] + p[j]) % 16;
            bg[i] = (fg[i] + p[k]) % 16;
        }
        j = (j + 3) % 7;
        k = (k + 5) % 7;
        // tty0.default_bg = bg[3];
        tty0.default_fg = fg[3];
        for (i = 0; i <= 2; ++i) {
            // ttys[i].default_bg = bg[i];
            ttys[i].default_fg = fg[i];
        }
    }
    // tprints(&tty1, "whats up");
    while (1) {
        asm volatile("hlt");
    }
    return;
}
#endif
