#ifndef PAGING_C
#define PAGING_C

#include "./include/paging.h"
#include "../../common/include/gsyms.h"
#include "./include/memory.h"
static inline uint64_t* get_next_table(uint64_t* pte, uint64_t voff) {
    uint64_t* pte_vaddr = (uint64_t*)(uint64_t)(((uint64_t)pte) & ~(PAGE_SIZE - 1) + voff);
    uint64_t* ret_vaddr;
    if (*pte_vaddr & PAGE_PRESENT) {
        ret_vaddr = (uint64_t*)(*pte_vaddr & ~(PAGE_SIZE - 1));
    } else {
        uint64_t pframe = (uint64_t)pmm_alloc();
        void* vframe = (void*)(pframe + voff);
        MemSet64(vframe, 0, PAGE_SIZE);
        *pte_vaddr = pframe | PAGE_DEFAULT;
        ret_vaddr = (uint64_t*)pframe;
    }
    return ret_vaddr;
}

void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags) {
    uint64_t pt_i = (virt >> 12) & PAGE_IDX_MASK;
    uint64_t pdt_i = (virt >> 21) & PAGE_IDX_MASK;
    uint64_t pdtp_i = (virt >> 30) & PAGE_IDX_MASK;
    uint64_t pml4_i = (virt >> 39) & PAGE_IDX_MASK;
    uint64_t *pdtp, *pdt, *pt;
    pdtp = get_next_table(&pml4[pml4_i], voff);
    pdt = get_next_table(&pdtp[pdtp_i], voff);
    pt = get_next_table(&pdt[pdt_i], voff);
    pt[pt_i] = phys | flags;
    return;
}

#endif
