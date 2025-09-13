#ifndef IO_H
#define IO_H
#include "tty.h"
extern tty_t tty0;
void kputchar(char c);
void kprints(char* str);
#endif
