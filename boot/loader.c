#ifndef LOADER_C
#define LOADER_C

#include <loader_mmu.h>
#include <utils.h>

extern char boot_stack_hi[];

void* loader_main(u32 __multiboot_paddr)
{
	struct mb_info* info = MAKE_PTR(__multiboot_paddr);
	static struct far_ptr ret_ptr;
	void* cr3_paddr;

	if (!MBI_HAS_MEM(info))
		halt_forever();
	if (!MBI_HAS_MMAP(info))
		halt_forever();
	if (!MBI_HAS_MODS(info))
		halt_forever();

	init_kernel(info, &cr3_paddr);

	init_gdt();

	init_tss(boot_stack_hi);

	init_ia32e_registers(cr3_paddr);

	init_jump_pointer(&ret_ptr);

	return &ret_ptr;
}

#endif
