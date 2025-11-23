#ifndef VMM_H
#define VMM_H

#include "./kcommon.h"

typedef struct addr_space {
    uint64_t* pml4;
    struct vmm_ent* start;
    struct vmm_ent* end;
} addr_space;

typedef struct vmm_ent {
    uint64_t start;
    uint64_t end;
    uint64_t flags;
    struct vmm_ent* next;
    uint64_t phys;
} vmm_ent;

#endif
