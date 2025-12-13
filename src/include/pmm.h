#ifndef PMM_H
#define PMM_H
#include <multiboot.h>

void init_pmm(mb_info* info);
uintptr_t pmm_alloc();
int pmm_free(uintptr_t page);

#endif
