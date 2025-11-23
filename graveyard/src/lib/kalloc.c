#ifndef KALLOC_C
#define KALLOC_C

#include "./include/kalloc.h"
#include "./include/io.h"
#include "./include/paging.h"
#include "./include/pmm.h"

uint64_t kheap_top;
heap_md *head = NULL;
void Init_Kernel_Heap(uint64_t *pml4)
{
	kheap_top = __KHEAP_TOP_VADDR__;
	void *alloc = pmm_alloc();
	map_memory(kheap_top, (uint64_t)alloc, pml4, HHDM_OFF, PAGE_DEFAULT);
}
void *kmalloc(uint32_t n)
{
	uint64_t free_addr = kheap_top;
	if (kheap_top + n > __KHEAP_TOP_VADDR__ + PAGE_SIZE) {
		/* TODO: interrupt handling and lazy paging */

		__KERNEL_PANIC__;
	}
	heap_md *ptr = (heap_md *)free_addr;
	ptr->free = 0;
	ptr->next = head;
	ptr->sz = n;
	ptr->checksum = HEAP_MAGIC;
	head = ptr;
	kheap_top += (n + sizeof(heap_md));
	return (void *)(free_addr + sizeof(heap_md));
}
void kfree(void *node)
{
	heap_md *md = (heap_md *)node - 1;
	if (!(md->checksum == HEAP_MAGIC)) {
		/* corrupted heap */
		__KERNEL_PANIC__;
	}
	md->free = 1;
}

#endif
