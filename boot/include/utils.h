#ifndef UTILS_H
#define UTILS_H

#include <types.h>

void loader_memset(void* a, char ch, U32 n);

static inline void halt_forever()
{
	for (;;)
		asm volatile("hlt");
}

#endif
