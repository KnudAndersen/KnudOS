#ifndef VGA_H
#define VGA_H

#include <types.h>

#define VGA_MMIO ((void*)0x000B8000)

#define VGA_COLS (80)
#define VGA_ROWS (25)

enum VGA_COLORS {
	VGA_BLACK = 0,
	VGA_LIGHT_GRAY = 7,
	VGA_LIGHT_BLUE = 9,
};

#define VGA_FG_DEF   (VGA_LIGHT_GRAY)
#define VGA_BG_DEF   (VGA_BLACK)
#define VGA_ATTR_DEF ((VGA_BG_DEF < 4) | VGA_FG_DEF)

void vga_advance_cursor(u32* cursor);
void vga_putchar(char ch);
void vga_identity_map_mmio();

#endif
