#include <vga.h>

U32 vga_cursor = 0;

void vga_advance_cursor(U32* cursor)
{
	U32 tmp = *cursor + 1;
	if (tmp / (VGA_COLS * VGA_ROWS)) {
		*cursor = 0;
	} else {
		(*cursor)++;
	}
}

void vga_putchar(U8 ch)
{
	volatile U16* wr_ptr = VGA_MMIO;
	wr_ptr[vga_cursor] = (VGA_ATTR_DEF << 8) | ch;
	vga_advance_cursor(&vga_cursor);
}
