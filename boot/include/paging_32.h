#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

/*fixed x86-defined constants*/
#define PAGE_SIZE         (4ULL * KiB)
#define PAGE_ENTS         (512ULL)
#define PAGE_OFF_MASK     (0xFFFULL)
#define PAGE_IDX_MASK     (0x1FFULL)
/* size of bootstrap reserve memory segment*/

/* page table entry flags */
#define PAGE_PRESENT      (1ULL << 0)
#define PAGE_EXEC_DISABLE (1ULL << 63)
#define PAGE_READ_WRITE   (1ULL << 1)
#define PAGE_DEFAULT      (PAGE_PRESENT | PAGE_READ_WRITE)
/* x86-define paging setup */
#define CR0_PG            (1ULL << 31) /* paging enable */
#define CR4_PAE           (1ULL << 5)  /* physical addr extn */
#define IA32_EFER_ADDR    (0xC0000080) /* model specific register for LM*/
#define IA32_EFER_LME     (1 << 8)     /* long mode enable */

void* reserve_alloc_page(void* reserve, uint64_t* reserve_off);
void map_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags,
                void* reserve, uint64_t* reserve_off);
void unmap_memory(uint64_t virt, uint64_t phys, uint64_t* pml4, uint64_t voff, uint64_t flags);

#endif
