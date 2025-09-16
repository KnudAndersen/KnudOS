#ifndef TTY_H
#define TTY_H

#include "kcommon.h"
#include "vga.h"

#define TTY_DEFAULT_FG (VGA_COLOR_RED)
#define TTY_DEFAULT_BG (VGA_COLOR_BLACK)
#define TTY_SCROLL_UP  (1)
#define TTY_TAB_WIDTH  (4)

typedef struct virtual_terminal {
    uint16_t x;
    uint16_t y;
    uint16_t rows;
    uint16_t cols;
    uint16_t default_fg;
    uint16_t default_bg;
    uint32_t row_i;
    uint32_t col_i;
    uint16_t data[VGA_ROWS_MAX][VGA_COLS_MAX];
} tty_t;

extern volatile uint16_t* vga_text_mmio;
void vga_tty_render(tty_t* tty);
void tty_scroll(tty_t* t, uint32_t dir, uint32_t n);
void tty_init(tty_t* tty, uint16_t rows, uint16_t cols, uint16_t fg, uint16_t bg, uint16_t x,
              uint16_t y);
#endif
