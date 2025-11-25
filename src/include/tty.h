#ifndef TTY_H
#define TTY_H

#include <types.h>

#define TTY_KIND_VGA (1)

typedef void tty_putchar_func(U8);

struct tty {
	tty_putchar_func* write_func;
};

void init_tty(U32 kind, struct tty* tty);
void tty_write_char(struct tty* tty, char c);
void tty_write_string(struct tty* tty, char* src);

#endif
