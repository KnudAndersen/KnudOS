#ifndef VGA_H
#define VGA_H

#include "lib64_common.h"

#define VGA_BG_MASK   (0x7000)
#define VGA_FG_MASK   (0x0F00)
#define VGA_CHAR_MASK (0x00FF)
#define VGA_ROWS_MAX  (25)
#define VGA_COLS_MAX  (80)

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

uint16_t vga_get_attr(char c, uint8_t fg, uint8_t bg);

#endif
