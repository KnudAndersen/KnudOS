#ifndef PMM_H
#define PMM_H
#include <multiboot.h>

void init_pmm(struct mb_preserved* info, uintptr_t loader_end);

uintptr_t pmm_alloc();
int pmm_free(uintptr_t page);

#endif
