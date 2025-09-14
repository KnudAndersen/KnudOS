#ifndef PAGING_32_H
#define PAGING_32_H

#include "lib32_common.h"

/*fixed x86-defined constants*/
#define PAGE_SIZE         (4ULL * KiB)
#define PAGE_ENTS         (512ULL)
#define PAGE_OFF_MASK     (0xFFFULL)
#define PAGE_IDX_MASK     (0x1FFULL)
/* size of bootstrap reserve memory segment*/
#define PAGE_RESERVE_SIZE (16 * 4 * KiB)
/* page table entry flags */
#define PAGE_PRESENT      (1ULL << 0)
#define PAGE_EXEC_DISABLE (1ULL << 63)
#define PAGE_WRITE        (1ULL << 1)
#define PAGE_DEFAULT      (PAGE_PRESENT | PAGE_WRITE)

void* reserve_alloc_page();
void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags);
void unmap_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags);

#endif
