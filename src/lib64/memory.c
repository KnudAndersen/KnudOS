#ifndef MEMORY_C
#define MEMORY_C
#include "memory.h"

/* -----------------------------------------
 * physical memory manager
 * ----------------------------------------- */
uint64_t pmm_bitmap[PMM_ROWS] = {0};

void pmm_init(multiboot_info* mb) {
    // TODO: loop through mb->mmap and mark reserved physical
    // pages as such.
    // TODO: mark first 3MiB of memory as reserved.
    // TODO: mark physical addresses like APIC as reserved
}

static inline void pmm_set_used(uint32_t i, uint32_t j) {
    pmm_bitmap[j] |= (1 << (PMM_COLS - 1 - i));
}

/* frees the physical page containing paddr (physical address)*/
void pmm_free(const uint64_t paddr) {
    uint32_t page_idx = paddr / PAGE_SIZE;
    uint32_t i = page_idx % PMM_COLS;
    uint32_t j = page_idx / PMM_COLS;
    pmm_bitmap[j] &= ~(1 << (PMM_COLS - 1 - i));
}

/* allocates an available physical page and returns its phys addr*/
void* pmm_alloc() {
    uint64_t ret_addr = 0;
    uint32_t i = 0, j = 0;
    for (j = 0; j < PMM_ROWS; j++) {
        for (i = 0; i < PMM_COLS; i++) {
            if (!((pmm_bitmap[j] >> (PMM_COLS - 1 - i)) & 1)) {
                ret_addr = PAGE_SIZE * (PMM_COLS * j + i);
                pmm_set_used(i, j);
                return (void*)ret_addr;
            }
        }
    }
    return (void*)0;
}

/* -----------------------------------------
 * TODO: virtual memory manager
 * ----------------------------------------- */

#endif
