#ifndef KALLOC_H
#define KALLOC_H

#include "kcommon.h"
typedef struct heap_md {
	// TODO: separate
	uint32_t sz;
	uint32_t free;
	uint32_t checksum;
	struct heap_md *next;
} heap_md;
#define HEAP_MAGIC (0xBADBEEF)

void Init_Kernel_Heap(uint64_t *pml4);

void *kmalloc(uint32_t n);
void kfree(void *node);
void kprint_heap();
#endif
