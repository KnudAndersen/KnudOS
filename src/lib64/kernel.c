#include "io.h"
#include "tty.h"
#include "vga.h"

#include <stdint.h>

tty_t tty0 = (tty_t){0};

void kernel_main(uint32_t stack_lo, uint32_t stack_hi) {
    tty_init(&tty0, VGA_ROWS_MAX, VGA_COLS_MAX, TTY_DEFAULT_FG, TTY_DEFAULT_BG);
    vga_tty_render(&tty0);
    char c = 0, x = 0, y = 0;
    while (1) {
        kputchar('A' + c);
        tty0.default_fg = x;
        tty0.default_bg = y;
        x = (x + 1) % 15;
        y = (y + 1) % 17;
        c = (c + 1) % ('Z' - 'A' + 1);
    }
    while (1) {
        asm volatile("hlt");
    }
    return;
}
