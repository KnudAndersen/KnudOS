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

void tty_write_char(struct tty* tty, char c)
{
	tty->write_func(c);
}

void tty_write_string(struct tty* tty, char* src)
{
	char* p = src;
	while (*p) {
		tty->write_func(*p);
		p++;
	}
}
