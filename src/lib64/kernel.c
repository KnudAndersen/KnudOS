#include "io.h"
#include "tty.h"
#include "vga.h"

#include <stdint.h>

tty_t tty0 = (tty_t){0};

void kernel_main(uint32_t stack_lo, uint32_t stack_hi) {
    tty_init(&tty0, VGA_ROWS_MAX, VGA_COLS_MAX, TTY_DEFAULT_FG, TTY_DEFAULT_BG);
    vga_tty_render(&tty0);
    kprints("hello world!\nwhat is up?");
    while (1) {
        asm volatile("hlt");
    }
    return;
}
