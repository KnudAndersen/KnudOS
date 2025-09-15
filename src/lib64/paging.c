#ifndef PAGING_C
#define PAGING_C

#include "paging.h"
#include "memory.h"

void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags) {
    uint16_t page_off = ((virt >> 0) & 0xFFF);
    uint16_t pt_i = ((virt >> 12) & 0x1FF);
    uint16_t pdt_i = ((virt >> 21) & 0x1FF);
    uint16_t pdtp_i = ((virt >> 30) & 0x1FF);
    uint16_t pml4_i = ((virt >> 39) & 0x1FF);
    uint64_t* pdtp = (uint64_t*)(pml4[pml4_i] + voff);
    if (!((uintptr_t)pdtp & PAGE_PRESENT)) {
        pdtp = (uint64_t*)pmm_alloc();
    }
    uint64_t* pdt = (uint64_t*)(pdtp[pdtp_i] + voff);
    uint64_t* pt = (uint64_t*)(pdt[pdt_i] + voff);
}

#endif
