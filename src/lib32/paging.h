#ifndef PAGING_32_H
#define PAGING_32_H

#include "lib32_common.h"

/*fixed x86-defined constants*/
#define PAGE_SIZE         (4 * KiB)
#define PAGE_ENTS         (512)
#define PAGE_OFF_MASK     (0xFFF)
#define PAGE_IDX_MASK     (0x1FF)
/* size of bootstrap reserve memory segment*/
#define PAGE_RESERVE_SIZE (16 * PAGE_SIZE)
/* page table entry flags */
#define PAGE_PRESENT      (1 << 0)
#define PAGE_RDWR         (1 << 1)
#define PAGE_DEFAULT      (PAGE_PRESENT | PAGE_RDWR)

void* reserve_alloc_page();
void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags);
void unmap_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags);

#endif
