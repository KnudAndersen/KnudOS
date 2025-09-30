#ifndef PAGING_C
#define PAGING_C

#include "./include/paging.h"
#include "./include/pmm.h"

// accepts physical address of PTE
static inline uint64_t* get_next_table(uint64_t* pte_vaddr, uint64_t voff) {
    uint64_t* ret_vaddr;
    if (*pte_vaddr & PAGE_PRESENT) {
        uint64_t tmp = *pte_vaddr & ~(PAGE_SIZE - 1);
        ret_vaddr = (uint64_t*)(tmp + voff);
    } else {
        uint64_t pframe = (uint64_t)pmm_alloc();
        void* vframe = (void*)(pframe + voff);
        MemSet64(vframe, 0, PAGE_SIZE);
        *pte_vaddr = pframe | PAGE_DEFAULT;
        ret_vaddr = (uint64_t*)vframe;
    }
    return ret_vaddr;
}

void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags) {
    uint64_t pt_i = (virt >> 12) & PAGE_IDX_MASK;
    uint64_t pdt_i = (virt >> 21) & PAGE_IDX_MASK;
    uint64_t pdtp_i = (virt >> 30) & PAGE_IDX_MASK;
    uint64_t pml4_i = (virt >> 39) & PAGE_IDX_MASK;
    uint64_t *pdtp, *pdt, *pt;
    // TODO: adjust pml4 to virtual address beforehand
    uint64_t* pml4_vaddr = (uint64_t*)((char*)pml4 + voff);
    pdtp = get_next_table(&pml4_vaddr[pml4_i], voff);
    pdt = get_next_table(&pdtp[pdtp_i], voff);
    pt = get_next_table(&pdt[pdt_i], voff);
    pt[pt_i] = phys | flags;
    return;
}

#endif
