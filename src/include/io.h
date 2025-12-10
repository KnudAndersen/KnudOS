#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include <types.h>
#include <tty.h>

void tprintf(const struct tty* tty, const char* str, va_list args);
void printf(const char* str, ...);

#endif
