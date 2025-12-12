#ifndef ASM_H
#define ASM_H

static inline void halt_forever()
{
	while (1)
		asm volatile("hlt");
}

#endif
