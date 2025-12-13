#include <types.h>
#include <asm.h>
#include <pmm.h>
#include <memlayout.h>
#include <vmm.h>
#include <string.h>
#include <page_table.h>

typedef struct vm_object {
	uintptr_t base;
	size_t length;
	u64 flags;
	link_list list_node;
} vm_object;

// TODO: support user space VMM's as well

void initialize_higher_half_direct_map()
{
	uintptr_t root_paddr = pop_cr3();

	size_t to_map = CEIL_DIV(MAX_PHYS_RAM, PAGE_SIZE) * PAGE_SIZE;
	u64 flags = VMM_OBJECT_FLAG_WRITE;

	// voff is 0 because we reserved identity map in boot sequence
	// see BOOT_IDENTITY_MAP_EXTRA in common/include/params.h
	for (size_t off = 0; off < to_map; off += PAGE_SIZE) {
		if (0xF00000 <= off && off <= 0xFFFFFF)
			continue;
		if (0xC0000000 <= off && off <= 0xFFFFFFFF)
			continue;
		x86_map_memory(HHDM_VOFF + off, off, root_paddr, flags, 0);
	}
}

void init_vmm(mb_info* info, vmm_type_t type, vm_address_space* addr_space)
{
	if (type == VMM_USER)
		halt_forever();

	uintptr_t root_paddr = pop_cr3();
	addr_space->list_head.next = addr_space->list_head.prev = NULL;
	addr_space->root = (page_table_t*)root_paddr;

	vm_object* vma_list = (vm_object*)(pmm_alloc);

	for (int stack_index = 0; stack_index < KSTACK_MAX_NUM; stack_index++) {
		vma_list[stack_index].base = KSTACK_NTH_LO(stack_index);
		vma_list[stack_index].length = KSTACK_SIZE;
		vma_list[stack_index].flags = VMM_OBJECT_FLAG_WRITE;
		link_list_insert(&vma_list[stack_index].list_node, &addr_space->list_head);
	}

	return;
}
