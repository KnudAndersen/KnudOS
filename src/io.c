#include <io.h>
#include <stdarg.h>
#include <str.h>

#define MAX_NIBBLES (16)
#define MAX_DECIMAL (20)

extern struct tty global_tty;

/* converts integer of given base to ascii rep stored in buf
 * in: [buf=dest buffer], [size=size of buf], [val=val to convert], [base=given base] 
 * out: pointer to start of buf where string is stored
 * notes: null terminates the string writing to index size-1
 */
static char* U64_to_base(char* buf, size_t size, U64 val, U32 base)
{
	const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* p = buf + size - 1;

	*p = '\0';

	if (val == 0) {
		*(--p) = '0';
		return p;
	}

	while (buf < p && val) {
		*(--p) = digits[(U32)(val % base)];
		val /= base;
	}

	if (val) {
		return NULL;
	}

	return p;
}

void PrintF(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	char fmt_buf[MAX_DECIMAL + 1];
	char* p;

	size_t i = 0, n = StrLen(fmt);
	while (i < n) {
		if (i < n - 1 && fmt[i] == '%') {
			switch (fmt[i + 1]) {
			case 'x':
				p = U64_to_base(fmt_buf, MAX_NIBBLES + 1, va_arg(ap, U64), 16);
				tty_write_string(&global_tty, p);
				i++;
				break;
			case 'd':
				p = U64_to_base(fmt_buf, MAX_DECIMAL + 1, va_arg(ap, U64), 10);
				tty_write_string(&global_tty, p);
				i++;
				break;
			default:
				return; // undefined behvaior
			}
		} else {
			tty_write_char(&global_tty, fmt[i]);
		}
		i++;
	}
	int arg1 = va_arg(ap, int);

	va_end(ap);
}
