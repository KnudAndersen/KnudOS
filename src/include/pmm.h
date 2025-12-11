#ifndef PMM_H
#define PMM_H
#include <multiboot.h>

void init_pmm(mb_info* info, uintptr_t loader_end);
uintptr_t pmm_alloc();
void pmm_free(uintptr_t page);

#endif
