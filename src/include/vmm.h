#ifndef VMM_H
#define VMM_H

#include <container.h>
#include <mmu.h>
#include <multiboot.h>

// NOTE: see 'static u64 x86_from_vmm_flags(u64 flags)' in page_table.h
// 	 for usage of these macros
#define VMM_OBJECT_FLAG_NONE  0
#define VMM_OBJECT_FLAG_WRITE (1 << 0)
#define VMM_OBJECT_FLAG_EXEC  (1 << 1)
#define VMM_OBJECT_FLAG_USER  (1 << 2)

enum vmm_type_t {
	VMM_KERNEL,
	VMM_USER,
};

struct vm_address_space {
	page_table_t* root;
	link_list list_head;
};

void init_vmm(struct mb_preserved* multiboot, enum vmm_type_t type,
              struct vm_address_space* addr_space);

#endif
