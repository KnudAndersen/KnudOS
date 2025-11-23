#ifndef ASM_H
#define ASM_H

#include <types.h>
#include <mmu.h>
#include <params.h>

static inline void load_gdt_register(struct gdtr* reg, struct gdt* gdt)
{
	*reg = (struct gdtr){
		sizeof(struct gdt) - 1,
		(UPTR)gdt,
	};
	asm volatile("lgdt %0" : : "m"(*reg) :);
}

static inline void load_task_register(U16 selector)
{
	asm volatile("mov ax, %0\n\t"
	             "ltr ax"
	             :
	             : "r"(selector)
	             :);
}

enum ctrl_reg {
	CR0,
	CR1,
	CR2,
	CR3,
	CR4,
};

static inline void write_ctrl_reg(enum ctrl_reg reg, U32 val)
{
	switch (reg) {
	case CR0:
		asm volatile("mov cr0, %0" : : "r"(val) : "memory");
		break;
	case CR1:
		asm volatile("mov cr1, %0" : : "r"(val) : "memory");
		break;
	case CR2:
		asm volatile("mov cr2, %0" : : "r"(val) : "memory");
		break;
	case CR3:
		asm volatile("mov cr3, %0" : : "r"(val) : "memory");
		break;
	case CR4:
		asm volatile("mov cr4, %0" : : "r"(val) : "memory");
		break;
	default:
		break;
	};
}

static inline void read_ctrl_reg(enum ctrl_reg reg, U64* ret)
{
	U32 tmp;
	switch (reg) {
	case CR0:
		asm volatile("mov %0, cr0" : "=r"(tmp) : : "memory");
		break;
	case CR2:
		asm volatile("mov %0, cr2" : "=r"(tmp) : : "memory");
		break;
	case CR3:
		asm volatile("mov %0, cr3" : "=r"(tmp) : : "memory");
		break;
	case CR4:
		asm volatile("mov %0, cr4" : "=r"(tmp) : : "memory");
		break;
	default:
		break;
	};
	*ret = tmp;
}

static inline void write_msr_register(U32 msr, U64 val)
{
	U32 high = (val >> 32) & 0xFFFFFFFF;
	U32 low = val & 0xFFFFFFFF;
	asm volatile("wrmsr" : : "c"(msr), "d"(high), "a"(low) : "memory");
}

static inline void read_msr_register(U32 msr, U64* ret)
{
	U32 high, low;
	asm volatile("rdmsr\n\t" : "=d"(high), "=a"(low) : "c"(msr) :);
	*ret = ((U64)high << 32) | low;
}
#endif
