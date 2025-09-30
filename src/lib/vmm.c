#ifndef VMM_C
#define VMM_C

#include "./include/vmm.h"
#include "./include/kalloc.h"
#include "./include/paging.h"
#include "./include/pmm.h"

void vmm_init() {
}

void* vmm_alloc(addr_space* as, uint64_t size, uint64_t x86_flags, void* arg) {
    /* TODO: enforce policy of start and end */
    /* TODO: have layer between x86 flags and vmm flags, makes stuff like XD easier*/
    vmm_ent* new_region = NULL;
    vmm_ent* prev = as->start;
    vmm_ent* itr = as->start->next;
    while (itr) {
        if (prev->end + size < itr->start) {
            new_region = kmalloc(sizeof(vmm_ent));
            *new_region = (vmm_ent){prev->end + 1, prev->end + 1 + size, x86_flags, itr};
            uint64_t* phys = (arg) ? arg : pmm_alloc();
            map_memory(prev->end + 1, (uint64_t)phys, as->pml4, HHDM_OFF, x86_flags);
            prev->next = new_region;
            break;
        }
    }

    return new_region;
}
void vmm_free() {
}

#endif
