#ifndef ASM_H
#define ASM_H
#include <types.h>

static inline void halt_forever()
{
	while (1)
		asm volatile("hlt");
}

#define CPUID_EXT_FEATURES (0x80000001)

static inline void cpuid(u32 function, u32* eax, u32* ebx, u32* ecx, u32* edx)
{
	asm volatile("cpuid"
	             : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
	             : "a"(function)
	             : "memory");
}

static inline u64 pop_cr3()
{
	u64 ret;
	asm volatile("mov %0, cr3" : "=r"(ret) : :);
	return ret;
}

static inline void invlpg(uintptr_t virt)
{
	asm volatile("invlpg [%0]" ::"r"(virt) : "memory");
}

#endif
