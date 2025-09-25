#ifndef GSYMS_C
#define GSYMS_C

// #include <stdint.h>

#include "./include/gsyms.h"

const uint64_t __KSTACK_TOP_VADDR__ = KSTACK_TOP;
const uint64_t __KSTACK_BASE_SIZE__ = 16 * KiB;
const uint64_t __KHEAP_TOP_VADDR__ = KSTACK_TOP - KSTACK_MAX - PAGE_SIZE;

#endif
