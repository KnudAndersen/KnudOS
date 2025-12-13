#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <types.h>
#include <asm.h>

#include <pmm.h> // NEED PHYSICAL PAGES => PMM NEEDS TO BE INITIALIZED ASAP
#include <mmu.h> // X86-SPECIFIC MACROS

void x86_map_memory(u64 virt, u64 phys, uintptr_t root_paddr, u64 vmm_flags, u64 voff);

#endif
