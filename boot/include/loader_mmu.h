#ifndef LOADER_MMU_H
#define LOADER_MMU_H

#include <multiboot.h>

struct far_ptr {
	u32 location; // RIP in 64-bit mode, IP in 16-bit mode
	u16 sel;      // CS value (16-bit)
} PACK;

void init_gdt();

void init_tss(void* stack_top);

void init_kernel(mb_info* info, void** cr3);

void init_ia32e_registers(void* cr3);

void init_jump_pointer(struct far_ptr* ret_ptr);

#endif
