#ifndef PAGING_C
#define PAGING_C

#include "./include/paging.h"
#include "./include/gsyms.h"

// assumes that boot_reserve is zero-d out for performance
void* reserve_alloc_page(void* boot_reserve, uint64_t* reserve_off) {
    uint64_t tmp = *reserve_off;
    if (tmp + PAGE_SIZE > __BOOT_RESERVE_SIZE__) {
        while (1) {
            asm volatile("hlt");
        }
        return (void*)0;
    }
    void* out = (void*)((char*)boot_reserve + tmp);
    *reserve_off += PAGE_SIZE;
    return out;
}

// walks page tables, allocates if necessary
// table_ent is a pointer to the page table entry
// , i.e. virtual address pointing to physical address of next table base
// returns virtual address of the start of next page in memory
static inline uint64_t* get_next_table(uint64_t* pte, uint64_t voff, void* reserve,
                                       uint64_t* reserve_off) {
    uint64_t* ret;
    if (*pte & PAGE_PRESENT) {
        uint64_t phys_addr = *pte & ~(PAGE_SIZE - 1);
        ret = (uint64_t*)(phys_addr + voff);
    } else {
        ret = (uint64_t*)reserve_alloc_page(reserve, reserve_off);
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

void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4_vaddr, uint64_t voff, uint64_t flags,
                void* reserve, uint64_t* reserve_off) {
    uint16_t pt_i = (virt >> 12) & PAGE_IDX_MASK;
    uint16_t pdt_i = (virt >> 21) & PAGE_IDX_MASK;
    uint16_t pdtp_i = (virt >> 30) & PAGE_IDX_MASK;
    uint16_t pml4_i = (virt >> 39) & PAGE_IDX_MASK;
    uint64_t *pdtp, *pdt, *pt;
    pdtp = get_next_table(&(pml4_vaddr[pml4_i]), voff, reserve, reserve_off);
    pdt = get_next_table(&(pdtp[pdtp_i]), voff, reserve, reserve_off);
    pt = get_next_table(&(pdt[pdt_i]), voff, reserve, reserve_off);
    pt[pt_i] = phys | flags;
}
#endif
