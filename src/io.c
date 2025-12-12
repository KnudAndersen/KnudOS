#include <io.h>
#include <string.h>

const struct tty* base_tty;

#define BUF_SIZE (20)

void init_io(const struct tty* tty)
{
	base_tty = tty;
}

static int format_numeric(char buf[BUF_SIZE], u64 val, int base)
{
	if (base < 0 || base > 36)
		return -1;

	if (!val) {
		buf[BUF_SIZE - 1] = '0';
		return 1;
	}

	const char alph[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int i = BUF_SIZE - 1;
	while (val && i > 0) {
		buf[i--] = alph[val % base];
		val /= base;
	}

	return (BUF_SIZE - 1) - (i);
}

static int format_isnumeric(char format_id, int* base, int* depth)
{
	char tmp[] = { format_id, '\0' };
	char* numeric_formats[] = { "X", "d", "D", 0 };
	int base_conversions[] = { 16, 10, 10 };
	for (int i = 0; numeric_formats[i]; i++)
		if (!strcmp(numeric_formats[i], tmp)) {
			*base = base_conversions[i];
			*depth = strlen(numeric_formats[i]);
			return 1;
		}
	return 0;
}

// i=index of % in str
static int handle_percent(const struct tty* tty, const char* str, int i, u64 arg)
{
	char buf[BUF_SIZE];
	int bytes_written;
	int base;
	int depth;

	if (format_isnumeric(str[i + 1], &base, &depth))
		bytes_written = format_numeric(buf, arg, base);
	else // TODO
		return 0;

	for (int i = 0; i < bytes_written; i++) {
		tty->write_func(buf[i + BUF_SIZE - bytes_written]);
	}

	return depth;
}

void tprintf(const struct tty* tty, const char* str, va_list args)
{
	for (int i = 0; str && str[i]; i++) {
		if (str[i] == '%' && str[i + 1])
			i += handle_percent(tty, str, i, va_arg(args, u64));
		else
			tty->write_func(str[i]);
	}
}

void printf(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	tprintf(base_tty, str, ap);
	va_end(ap);
}
