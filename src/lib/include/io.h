#ifndef IO_H
#define IO_H

#include "kcommon.h"
#include "tty.h"

#define HEXSHIFT (4)
#define HEXMOD   (0xF)
#define DECMOD   (10)

void kprintc(char c);
void kprints(char* str);
void kprintb(uint8_t val);
void kprintd(uint32_t val);
void kprintl(uint64_t val);
void kprintbx(uint8_t val);
void kprintdx(uint32_t val);
void kprintlx(uint64_t val);

void tprintc(tty_t* tty, char c);
void tprints(tty_t* tty, char* str);
void tprintb(tty_t* tty, uint8_t val);
void tprintd(tty_t* tty, uint32_t val);
void tprintl(tty_t* tty, uint64_t val);
void tprintbx(tty_t* tty, uint8_t val);
void tprintdx(tty_t* tty, uint32_t val);
void tprintlx(tty_t* tty, uint64_t val);

#endif
