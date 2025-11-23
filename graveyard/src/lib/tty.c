#ifndef TTY_C
#define TTY_C

#include "./include/tty.h"
#include "./include/io.h"

static const char start_msg[] = "[TTY] Initialized tty";

#define MAX_TTY (4)
tty_t *ttys[MAX_TTY];

void Init_TTY(tty_t *tty, uint16_t rows, uint16_t cols, uint16_t fg,
	      uint16_t bg, uint16_t x, uint16_t y)
{
	static uint8_t tty_id = 0;
	if (tty_id == MAX_TTY) {
		__KERNEL_PANIC__;
		return;
	}
	ttys[tty_id] = tty;
	tty->x = x;
	tty->y = y;
	tty->rows = rows;
	tty->cols = cols;
	tty->default_bg = bg;
	tty->default_fg = fg;
	tty->row_i = (tty->col_i = 1);
	for (uint32_t i = 0; i < rows; i++) {
		tty->data[i][0] = vga_get_attr('|', fg, bg);
		tty->data[i][cols - 1] = vga_get_attr('|', fg, bg);
	}
	for (uint32_t i = 0; i < cols; i++) {
		tty->data[0][i] = vga_get_attr('-', fg, bg);
		tty->data[rows - 1][i] = vga_get_attr('-', fg, bg);
	}
	tbuffers(tty, start_msg);
	tbufferd(tty, tty_id);
	tbufferc(tty, '\n');
	tty_id++;
}

static inline void vga_tty_render_h(tty_t *tty, uint32_t xoff, uint32_t yoff)
{
	// frame location
	uint32_t rc = tty->rows;
	uint32_t cc = tty->cols;
	uint32_t vga_idx;
	for (uint32_t j = 0; j < rc; ++j) {
		for (uint32_t i = 0; i < cc; ++i) {
			vga_idx = (j + yoff) * VGA_COLS_MAX + (i + xoff);
			vga_text_mmio[vga_idx] = tty->data[j][i];
		}
	}
}
void vga_tty_render(tty_t *tty)
{
	vga_tty_render_h(tty, tty->x, tty->y);
}

void tty_scroll(tty_t *t, uint32_t dir, uint32_t n)
{
	if (dir == TTY_SCROLL_UP) {
		uint32_t cc = t->cols;
		uint32_t rc = t->rows;
		for (uint32_t j = 1; j < cc - 1; ++j) {
			for (uint32_t i = 1; i < rc - n - 1; ++i) {
				t->data[i][j] = t->data[i + n][j];
			}
		}
	}
	vga_tty_render(t);
}
#endif
