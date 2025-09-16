#ifndef TTY_C
#define TTY_C

#include "./include/tty.h"
#include "./include/io.h"

static inline void set_start_message(tty_t* tty, uint8_t id) {
    // TODO buffered stdio
    kprints("[TTY] Initialized tty");
    kprintb(id);
    kprintc('\n');
}

void tty_init(tty_t* tty, uint16_t rows, uint16_t cols, uint16_t fg, uint16_t bg, uint16_t x,
              uint16_t y) {
    static uint8_t tty_id = 0;
    tty->x = x;
    tty->y = y;
    tty->rows = rows;
    tty->cols = cols;
    tty->default_bg = bg;
    tty->default_fg = fg;
    tty->row_i = (tty->col_i = 1);
    uint16_t tmp = rows * cols;
    for (uint32_t i = 1; i < tmp; i++)
        *((uint16_t*)(tty->data) + i) = vga_get_attr(' ', fg, bg);

    // set_start_message(tty, tty_id);
    for (uint32_t i = 0; i < rows; i++) {
        tty->data[i][0] = vga_get_attr('x', fg, bg);
    }
    for (uint32_t i = 0; i < cols; i++) {
        tty->data[0][i] = vga_get_attr('x', fg, bg);
    }
    ++tty_id;
}

static inline void vga_tty_render_h(tty_t* tty, uint32_t xoff, uint32_t yoff) {
    uint32_t rc = tty->rows;
    uint32_t cc = tty->cols;
    uint32_t vga_idx;
    for (uint32_t j = 0; j < rc; ++j) {
        for (uint32_t i = 0; i < cc; ++i) {
            vga_idx = (yoff + j) * cc + (xoff + i);
            vga_text_mmio[vga_idx] = tty->data[j][i];
        }
    }
}
void vga_tty_render(tty_t* tty) {
    vga_tty_render_h(tty, tty->x, tty->y * VGA_COLS_MAX);
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
