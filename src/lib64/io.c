#ifndef IO_C
#define IO_C

#include "io.h"

extern tty_t tty0;

void kputchar(char c) {
    tty_t* term = &tty0;
    if (term == 0) {
        while (1) {
            asm volatile("hlt");
        }
    }
    if (c == '\t') {
        for (uint8_t i = 0; i < TTY_TAB_WIDTH; i++)
            kputchar(' ');
        return;
    }
    if (term->col_i == term->cols || c == '\n') {
        term->col_i = 0;
        term->row_i++;
        if (term->row_i == term->rows) {
            tty_scroll(term, TTY_SCROLL_UP, 1);
            term->row_i = term->rows - 1;
        }
    }
    if (c != '\n' && c != '\t') {
        term->data[term->row_i][term->col_i++] =
            vga_get_attr(c, term->default_fg, term->default_bg);
        vga_tty_render(&tty0);
    }
}
void kprints(char* str) {
    uint32_t i = 0;
    while (str[i]) {
        kputchar(str[i]);
        i++;
    }
}
#endif
