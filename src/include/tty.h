#ifndef TTY_H
#define TTY_H
#include <types.h>

// #define TTY_KIND_VGA  (1)
#define TTY_TABLENGTH (4)
typedef void tty_putchar_func(char);

typedef enum {
	TTY_KIND_VGA = 0,
} tty_kind_t;

struct tty {
	tty_putchar_func* write_func;
};

void init_tty(tty_kind_t kind, struct tty* tty);

#endif
