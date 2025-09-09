#ifndef PAGING_32_H
#define PAGING_32_H

#include "lib32_common.h"

/*fixed x86-defined constants*/
#define PAGE_SIZE           (4096)
#define PAGE_ENTS           (512)   /*num ents in page table*/
#define PAGE_OFF_MASK       (0xFFF) /*mask for offset into phys page*/
#define PAGE_IDX_MASK       (0x1FF) /*mask for index into page table*/

/*number of page tables for boostrapping*/
#define PG_STRUC_STATIC_NUM (256 + 1 + 1)

/* page table entry flags */
#define PG_PRESENT          (1 << 0)
#define PG_RDWR             (1 << 1)
#define PG_DEFAULT_FLAGS    (PG_PRESENT | PG_RDWR)

void map_memory(uint64_t virt, uint64_t phys, uint64_t flags);

#endif
