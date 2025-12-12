#include <vga.h>
#include <string.h>
#include <tty.h>

u32 vga_cursor = 0;
u16 virtual_screen[VGA_COLS * VGA_ROWS] = { 0 };

void vga_shift_up(u16* virt, u32 num)
{
	u32 row_src, row_dst;
	for (u32 i = num; i < VGA_ROWS; i++) {
		row_src = i * VGA_COLS;
		row_dst = (i - num) * VGA_COLS;
		memcpy(&virt[row_dst], &virt[row_src], VGA_COLS * sizeof(u16));
	}
	memset(&virt[(VGA_ROWS - num) * VGA_COLS], 0, num * VGA_COLS * sizeof(u16));
}

// TODO: double buffering / optimization
void vga_render(u16* virt)
{
	volatile u16* mmio = VGA_MMIO;
	memcpy_v(mmio, virt, VGA_ROWS * VGA_COLS * sizeof(u16));
}

void vga_advance_cursor(u32* cursor)
{
	if ((*cursor + 1) / (VGA_COLS * VGA_ROWS)) {
		vga_shift_up(virtual_screen, 1);
		vga_render(virtual_screen);
		*cursor = (VGA_ROWS - 1) * VGA_COLS;
	} else {
		(*cursor)++;
	}
}

void vga_putchar(char ch)
{
	volatile u16* mmio = VGA_MMIO;
	switch (ch) {
		case '\t':
			for (u8 i = 0; i < TTY_TABLENGTH; i++)
				vga_putchar(' ');
			break;
		case '\n':
			vga_cursor += (VGA_COLS - 1 - (vga_cursor % VGA_COLS));
			vga_advance_cursor(&vga_cursor);
			break;
		case '\0':
			break;
		default:
			virtual_screen[vga_cursor] = (VGA_ATTR_DEF << 8) | ch;
			mmio[vga_cursor] = (VGA_ATTR_DEF << 8) | ch;
			vga_advance_cursor(&vga_cursor);
			break;
	}
}
