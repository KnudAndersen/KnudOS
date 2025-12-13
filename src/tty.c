#include <tty.h>
#include <vga.h>

void init_tty(tty_kind_t kind, struct tty* tty)
{
	switch (kind) {
		case TTY_KIND_VGA:
			tty->write_func = vga_putchar;
			vga_identity_map_mmio();
			break;
		default:
			break;
	}
}
