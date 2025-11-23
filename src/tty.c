#include <tty.h>
#include <vga.h>

void init_tty(U32 kind, struct tty* tty)
{
	switch (kind) {
		;
	case TTY_KIND_VGA:
		tty->write_func = vga_putchar;
		break;

	default:
		break;
	}
}
