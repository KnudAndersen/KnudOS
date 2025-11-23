#ifndef MEMORY_MANAGER_C
#define MEMORY_MANAGER_C

#include "include/pmm.h"

void Init_MemoryManager(multiboot_info *mb)
{
	Init_PMM(mb);
	// TODO: Init_VMM()
	return;
}

#endif
