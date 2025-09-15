#ifndef PAGING_H
#define PAGING_H

#include "lib64_common.h"

/*fixed x86-defined constants*/
#define PAGE_ENTS         (512ULL)
#define PAGE_OFF_MASK     (0xFFFULL)
#define PAGE_IDX_MASK     (0x1FFULL)
/* size of bootstrap reserve memory segment*/
#define PAGE_RESERVE_SIZE (16 * 4 * KiB)
/* page table entry flags */
#define PAGE_PRESENT      (1ULL << 0)
#define PAGE_EXEC_DISABLE (1ULL << 63)
#define PAGE_READ_WRITE   (1ULL << 1)
#define PAGE_DEFAULT      (PAGE_PRESENT | PAGE_READ_WRITE)

void map_memory(uint64_t virt, uint64_t phys, uint64_t* plm4, uint64_t voff, uint64_t flags);

#endif
