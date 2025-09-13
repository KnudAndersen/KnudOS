#ifndef VGA_C
#define VGA_C

#include "vga.h"

volatile uint16_t* vga_text_mmio = (volatile uint16_t*)0xB8000;

uint16_t vga_get_attr(char c, uint8_t fg, uint8_t bg) {
    return ((c << 0) & VGA_CHAR_MASK) | ((fg << 8) & VGA_FG_MASK) | ((bg << 12) & VGA_BG_MASK);
}

#endif
