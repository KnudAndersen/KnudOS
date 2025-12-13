#include <page_table.h>
#include <string.h>
#include <vmm.h>

static u64 x86_from_vmm_flags(u64 flags)
{
	u64 ret = X86_PRESENT;
	if (flags & VMM_OBJECT_FLAG_WRITE)
		ret |= X86_READ_WRITE;
	if (flags & VMM_OBJECT_FLAG_USER)
		ret |= X86_USER;
	// TODO: re-implement
	//	if (!(flags & VMM_OBJECT_FLAG_EXEC))
	//		ret |= X86_EXEC_DISABLE;
	return ret;
}

static uintptr_t x86_walk_pagetable(uintptr_t pt_paddr, u64 ptoff, u64 x86_flags, u64 voff)
{
	page_table_t* pt_vaddr = (page_table_t*)(pt_paddr + voff);

	// TODO: perform a check confirming sizeof(uinptr_t) == sizeof(u64) early in kernel
	uintptr_t next_table = pt_vaddr->entries[ptoff];

	if (!(next_table & X86_PRESENT)) {
		// NOTE: these need to be identity mapped by the boot sequence
		// we need to access these physical addresses for walking and scrubbing
		uintptr_t physalloc = pmm_alloc();
		memset((void*)physalloc, 0, PAGE_SIZE);
		next_table = pt_vaddr->entries[ptoff] = physalloc | x86_flags | X86_PRESENT;
	}

	return next_table & X86_ADDR_MASK;
}

// TODO: figure out best place to put x86_from_vmm_flags, vmm.h or page_table.h?
void x86_map_memory(u64 virt, u64 phys, uintptr_t root_paddr, u64 vmm_flags, u64 voff)
{
	if (phys % PAGE_SIZE || virt % PAGE_SIZE)
		halt_forever();

	u64 x86_flags = x86_from_vmm_flags(vmm_flags);

	u64 ptoff = (virt >> 12) & 0x1FF;
	u64 pdtoff = (virt >> 21) & 0x1FF;
	u64 pdptoff = (virt >> 30) & 0x1FF;
	u64 pml4off = (virt >> 39) & 0x1FF;

	uintptr_t pml4_base_paddr = root_paddr;
	uintptr_t pdpt_base_paddr = x86_walk_pagetable(pml4_base_paddr, pml4off, x86_flags, voff);
	uintptr_t pdt_base_paddr = x86_walk_pagetable(pdpt_base_paddr, pdptoff, x86_flags, voff);
	uintptr_t pt_base_paddr = x86_walk_pagetable(pdt_base_paddr, pdtoff, x86_flags, voff);

	page_table_t* pt_vaddr = (page_table_t*)(pt_base_paddr + voff);
	pt_vaddr->entries[ptoff] = phys | x86_flags | X86_PRESENT;
}
