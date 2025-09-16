#ifndef PAGING_32_C
#define PAGING_32_C

#include "./include/paging_32.h"

extern char boot_reserve[];
extern uint64_t reserve_off;
uint64_t* boot_pml4;

// assumes that boot_reserve is zero-d out for performance
void* reserve_alloc_page() {
    if (reserve_off + PAGE_SIZE > PAGE_RESERVE_SIZE) {
        while (1) {
            asm volatile("hlt");
        }
        return NULL;
    }
    void* out = (void*)(boot_reserve + reserve_off);
    reserve_off += PAGE_SIZE;
    return out;
}

// walks page tables, allocates if necessary
// table_ent is a pointer to the page table entry
// , i.e. virtual address pointing to physical address of next table base
// returns virtual address of the start of next page in memory
static inline uint64_t* get_next_table(uint64_t* pte, uint64_t voff) {
    uint64_t* ret;
    if (*pte & PAGE_PRESENT) {
        uint64_t phys_addr = *pte & ~(PAGE_SIZE - 1);
        ret = (uint64_t*)(phys_addr + voff);
    } else {
        ret = (uint64_t*)reserve_alloc_page();
        if (ret) {
            uint64_t phys_page = (uint64_t)((uintptr_t)ret - voff);
            *pte = phys_page | PAGE_DEFAULT;
        }
    }
    return ret;
}

void unmap_memory(uint64_t virt, uint64_t phys, uint64_t* pml4_vaddr, uint64_t voff,
                  uint64_t flags) {
    // TODO
    return;
}

void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4_vaddr, uint64_t voff, uint64_t flags) {
    uint16_t pt_i = (virt >> 12) & PAGE_IDX_MASK;
    uint16_t pdt_i = (virt >> 21) & PAGE_IDX_MASK;
    uint16_t pdtp_i = (virt >> 30) & PAGE_IDX_MASK;
    uint16_t pml4_i = (virt >> 39) & PAGE_IDX_MASK;
    uint64_t *pdtp, *pdt, *pt;
    pdtp = get_next_table(&(pml4_vaddr[pml4_i]), voff);
    pdt = get_next_table(&(pdtp[pdtp_i]), voff);
    pt = get_next_table(&(pdt[pdt_i]), voff);
    pt[pt_i] = phys | flags;
}
#endif
