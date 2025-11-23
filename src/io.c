#include <io.h>

VOID PrintF(struct tty* tty, CHAR* str)
{
	CHAR* tmp = str;
	while (*tmp) {
		tty->write_func(*tmp);
		tmp++;
	}
}
