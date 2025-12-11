#ifndef ASM_H
#define ASM_H

static inline void halt_forever()
{
	asm volatile("cli");
	while (1)
		asm volatile("hlt");
}

#endif
