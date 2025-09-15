#ifndef TTY_C
#define TTY_C

#include "tty.h"
#include "io.h"

static inline void set_start_message(tty_t* tty, uint8_t id) {
    kprints("[TTY] Initialized tty\n");
}

void tty_init(tty_t* tty, uint16_t rows, uint16_t cols, uint16_t fg, uint16_t bg) {
    static uint8_t tty_id = 0;
    tty_id++;
    tty->rows = rows;
    tty->cols = cols;
    tty->default_bg = bg;
    tty->default_fg = fg;
    tty->row_i = (tty->col_i = 0);
    rows *= cols;
    for (uint32_t i = 0; i < rows; i++)
        *((uint16_t*)(tty->data) + i) = vga_get_attr(' ', fg, bg);
    set_start_message(tty, tty_id);
}

void vga_tty_render(tty_t* tty) {
    uint32_t rc = tty->rows;
    uint32_t cc = tty->cols;
    for (uint32_t j = 0; j < rc; ++j) {
        for (uint32_t i = 0; i < cc; ++i) {
            vga_text_mmio[j * cc + i] = tty->data[j][i];
        }
    }
}

void tty_scroll(tty_t* t, uint32_t dir, uint32_t n) {
    if (dir == TTY_SCROLL_UP) {
        for (uint32_t j = 0; j < t->cols; ++j) {
            for (uint32_t i = 0; i < t->rows - n; ++i) {
                t->data[i][j] = t->data[i + n][j];
            }
        }
    }
    vga_tty_render(t);
}
#endif
