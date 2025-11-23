#ifndef PMM_H
#define PMM_H

#include "kcommon.h"

#define PMM_ROWS (165)
#define PMM_COLS (64)

void Init_PMM(multiboot_info *mb);
void pmm_free(const uint64_t addr);
void *pmm_alloc();

#endif
