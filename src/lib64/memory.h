#ifndef MEMORY_H
#define MEMORY_H

#include "lib64_common.h"

#define PMM_ROWS (8)
#define PMM_COLS (64)

void pmm_free(const uint64_t addr);
void* pmm_alloc();
#endif
