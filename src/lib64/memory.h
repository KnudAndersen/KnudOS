#ifndef MEMORY_H
#define MEMORY_H

#include "../common/multiboot.h"
#include "lib64_common.h"

#define PMM_ROWS (8)
#define PMM_COLS (64)

void pmm_init(multiboot_info* mb);
void pmm_free(const uint64_t addr);
void* pmm_alloc();
#endif
