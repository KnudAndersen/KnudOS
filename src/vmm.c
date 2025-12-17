#include <types.h>
#include <asm.h>
#include <pmm.h>
#include <vga.h>
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

void remove_boot_identity_mappings(struct mb_preserved* multiboot)
{
	uintptr_t root_paddr = pop_cr3();

	char* ptr = (char*)(uintptr_t)multiboot->mmap;
	char* end = ptr + multiboot->info.mmap_length;
	while (ptr < end) {
		struct mb_mmap_entry* entry = (struct mb_mmap_entry*)ptr;
		if (entry->type == MB_MMAP_AVAILABLE) {
			u64 virt = entry->base_addr;

			if (virt + entry->length >= HHDM_HI)
				halt_forever();
			x86_unmap_memory_range(virt, entry->length, root_paddr, HHDM_VOFF);
		}
		ptr += entry->size + sizeof(entry->size);
	}
}

// TODO: make a helper for the multiboot for loop
// NOTE: assumes that page tables are identity mapped at boot (see boot/loader_mmu.c)
void initialize_higher_half_direct_map(struct mb_preserved* multiboot)
{
	uintptr_t root_paddr = pop_cr3();

	char* ptr = (char*)(uintptr_t)multiboot->mmap;
	char* end = ptr + multiboot->info.mmap_length;

	while (ptr < end) {
		struct mb_mmap_entry* entry = (struct mb_mmap_entry*)ptr;
		if (entry->type == MB_MMAP_AVAILABLE) {
			u64 virt = entry->base_addr + HHDM_VOFF;
			u64 phys = entry->base_addr;

			// the HHDM needs to be elongated
			// to prevent virtual address corruption (stack gets hit first)
			if (virt + entry->length >= HHDM_HI)
				halt_forever();

			u64 vmm_flags = VMM_OBJECT_FLAG_WRITE;
			x86_map_memory_range(virt, phys, entry->length, root_paddr, vmm_flags, 0);
		}
		ptr += entry->size + sizeof(entry->size);
	}
}
// TODO: user space VMM support
void init_vmm(struct mb_preserved* multiboot, enum vmm_type_t type,
              struct vm_address_space* addr_space)
{
	if (type == VMM_USER)
		halt_forever();

	initialize_higher_half_direct_map(multiboot);
	remove_boot_identity_mappings(multiboot);

	// 	uintptr_t root_paddr = pop_cr3();
	// 	addr_space->list_head.next = addr_space->list_head.prev = NULL;
	// 	addr_space->root = (page_table_t*)root_paddr;
	//
	// 	vm_object* vma_list = (vm_object*)(pmm_alloc);
	//
	// 	for (int stack_index = 0; stack_index < KSTACK_MAX_NUM; stack_index++) {
	// 		vma_list[stack_index].base = KSTACK_NTH_LO(stack_index);
	// 		vma_list[stack_index].length = KSTACK_SIZE;
	// 		vma_list[stack_index].flags = VMM_OBJECT_FLAG_WRITE;
	// 		link_list_insert(&vma_list[stack_index].list_node, &addr_space->list_head);
	// 	}

	return;
}
