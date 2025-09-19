#ifndef MEMORY_C
#define MEMORY_C
#include "./include/memory.h"
#include "../common/include/gsyms.h"
#include "./include/io.h"

/* -----------------------------------------
 * physical memory manager
 * ----------------------------------------- */
uint64_t pmm_bitmap[PMM_ROWS] = {0};

static inline void pmm_set_used(uint32_t i, uint32_t j) {
    pmm_bitmap[j] |= (1 << (PMM_COLS - 1 - i));
}
static inline uint32_t pmm_used(uint32_t i, uint32_t j) {
    return (pmm_bitmap[j] >> (PMM_COLS - 1 - i)) & 1;
}
void pmm_reserve(uint64_t paddr, uint64_t n) {
    uint32_t end = ((paddr + n + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
    uint32_t i, j;
    uint32_t num_pages = CEIL_DIV(n, PAGE_SIZE);
    uint32_t start_page = paddr / PAGE_SIZE;
    uint32_t end_page = start_page + num_pages;
    for (uint32_t page_i = start_page; page_i < end_page; page_i++) {
        i = page_i % PMM_COLS;
        j = page_i / PMM_COLS;
        if (j >= PMM_ROWS) {
            __KERNEL_PANIC__;
        }
        pmm_set_used(i, j);
    }
}
void pmm_init(multiboot_info* mb) {
    if ((uint64_t)mb != 0x10000) {
        /* TODO: confirm this is not reachable */
        __KERNEL_PANIC__;
    }
    pmm_reserve(0, 2 * MiB);
    multiboot_mod* m_mod = get_kernel(mb);
    pmm_reserve(m_mod->mod_start, m_mod->mod_end - m_mod->mod_start);

    char* base = (char*)(uintptr_t)mb->mmap_addr;
    mb_mmap_ent* iter;
    uint32_t i = 0;
    uint32_t bytes_read = mb->mmap_length;
    uint32_t to_read = 0;
    while (bytes_read < to_read) {
        iter = (mb_mmap_ent*)(base + bytes_read);
        if (iter->base_addr + iter->length >= PAGE_SIZE * 8 * sizeof(pmm_bitmap)) {
            __KERNEL_PANIC__;
        }
        switch (iter->type) {
            case MULTIBOOT_MMAP_RESERVED:
                pmm_reserve(iter->base_addr, iter->length);
                break;
            case MULTIBOOT_MMAP_ACPI:
                pmm_reserve(iter->base_addr, iter->length);
                break;
            default:
                __KERNEL_PANIC__;
                break;
        }
        bytes_read += iter->size;
    }
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
            if (!pmm_used(i, j)) {
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

/* -----------------------------------------
 * TODO: kernel dyn alloc
 * ----------------------------------------- */
typedef struct heap_md {
    // TODO: separate
    uint32_t sz;
    uint32_t free;
    uint32_t checksum;
    struct heap_md* next;
} heap_md;
#define HEAP_MAGIC (0xBADBEEF)
uint64_t kheap_top;
heap_md head;
void init_kheap() {
    kheap_top = __KHEAP_TOP_VADDR__;
    head.next = NULL;
    head.free = 0;
    head.sz = 0;
    head.checksum = HEAP_MAGIC;
}
void kprint_heap() {
    heap_md* itr = &head;
    while (itr) {

        kprints("[INFO] DUMPING KERNEL HEAP METADATA\n");
        kprints("{");
        kprintlx(itr->sz);
        kprints(", ");
        kprintlx(itr->free);
        kprints(", ");
        kprintlx(itr->checksum);
        kprints(", ");
        kprintlx((uintptr_t)itr->next);
        kprints("}\n");
        itr = itr->next;
    }
    return;
}
void* kmalloc(uint32_t n) {
    uint64_t free_addr = kheap_top;
    heap_md* ptr = (heap_md*)free_addr;
    ptr->free = 0;
    ptr->next = &head;
    ptr->sz = n;
    ptr->checksum = HEAP_MAGIC;
    kheap_top += (n + sizeof(heap_md));
    return (void*)(free_addr + sizeof(heap_md));
}
void kfree(void* node) {
    heap_md* md = (heap_md*)node - 1;
    if (!(md->checksum == HEAP_MAGIC)) {
        __KERNEL_PANIC__;
    }
    md->free = 1;
}
#endif
