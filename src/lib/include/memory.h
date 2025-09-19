#ifndef MEMORY_H
#define MEMORY_H

#include "../../common/include/multiboot.h"
#include "kcommon.h"

#define PMM_ROWS (8)
#define PMM_COLS (128)

void pmm_init(multiboot_info* mb);
void pmm_free(const uint64_t addr);
void* pmm_alloc();
#endif
