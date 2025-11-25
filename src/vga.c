#include <vga.h>

U32 vga_cursor = 0;
U16 vga_virtual[VGA_COLS * VGA_ROWS];

/* advances the sole vga-device cursor
 * returns 1 if the screen needs to be rotated 
 *              (i.e. no more unoccupied cells)
 * returns 0 otherwise
 */
static U32 vga_advance_cursor(U32* cursor)
{
	U32 tmp = *cursor + 1;
	if (tmp / (VGA_COLS * VGA_ROWS)) {
		*cursor = VGA_COLS * (VGA_ROWS - 1);
		return 1;
	}
	(*cursor)++;
	return 0;
}
/* shifts the virtual screen by shift and renders the result*/
static void vga_rot_vsc(U16* virtual, I32 shift)
{
	U32 i, j;
	for (i = 1; i < VGA_ROWS; i++) {
		for (j =)
	}
}

void vga_putchar(U8 ch)
{
	volatile U16* wr_ptr = VGA_MMIO;
	wr_ptr[vga_cursor] = (VGA_ATTR_DEF << 8) | ch;
	vga_virtual[vga_cursor] = wr_ptr[vga_cursor];
	if (vga_advance_cursor(&vga_cursor)) {
		vga_rot_vsc(vga_virtual, 1);
	}
}
