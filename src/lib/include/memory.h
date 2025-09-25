#ifndef MEMORY_H
#define MEMORY_H

#include "../../../common/include/multiboot.h"
#include "kcommon.h"

#define PMM_ROWS (165)
#define PMM_COLS (64)

typedef struct heap_md {
    // TODO: separate
    uint32_t sz;
    uint32_t free;
    uint32_t checksum;
    struct heap_md* next;
} heap_md;
#define HEAP_MAGIC (0xBADBEEF)

void pmm_init(multiboot_info* mb);
void pmm_free(const uint64_t addr);
void* pmm_alloc();
void init_kheap(uint64_t* pml4);
void* kmalloc(uint32_t n);
void kfree(void* node);
#endif
