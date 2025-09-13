#ifndef GDT_32_C
#define GDT_32_C
#include "gdt.h"

gdtr32_t gdtr __attribute__((aligned(8)));
gdt_desc gdt[GDT_NUM_ENT] __attribute__((aligned(8)));
uint32_t tss[TSS_NUM_ENT] __attribute__((aligned(8)));

gdt_desc set_gdt_desc(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    gdt_desc out = 0;
    uint64_t tmp;
    tmp = (limit >> 0) & GDT_LIM_LO_MASK;
    out |= (tmp << GDT_LIM_LO_SHIFT);
    tmp = (limit >> 16) & GDT_LIM_HI_MASK;
    out |= (tmp << GDT_LIM_HI_SHIFT);
    tmp = (base >> 0) & GDT_BASE_LO_MASK;
    out |= (tmp << GDT_BASE_LO_SHIFT);
    tmp = (base >> 24) & GDT_BASE_HI_MASK;
    out |= (tmp << GDT_BASE_HI_SHIFT);
    tmp = access;
    out |= (tmp << GDT_ACCESS_SHIFT);
    tmp = (flags >> 0) & GDT_FLAGS_MASK;
    out |= (tmp << GDT_FLAGS_SHIFT);
    return out;
}

void init_gdt() {
    for (int i = 0; i < GDT_NUM_ENT; ++i)
        gdt[i] = 0;
    gdt[GDT_KERN_CODE] = set_gdt_desc(0, 0xFFFFF, GDT_KERN_CODE_ACC, GDT_DEFAULT_FLAG);
    gdt[GDT_KERN_DATA] = set_gdt_desc(0, 0xFFFFF, GDT_KERN_DATA_ACC, GDT_DEFAULT_FLAG);
    gdt[GDT_USER_CODE] = set_gdt_desc(0, 0xFFFFF, GDT_USER_CODE_ACC, GDT_DEFAULT_FLAG);
    gdt[GDT_USER_DATA] = set_gdt_desc(0, 0xFFFFF, GDT_USER_DATA_ACC, GDT_DEFAULT_FLAG);
    gdtr.base = (uint32_t)(uintptr_t)&gdt;
    gdtr.limit = (uint32_t)(sizeof(gdt) - 1);
}
void init_tss(uint64_t stack_vaddr) {
    for (int i = 0; i < TSS_NUM_ENT; ++i)
        tss[i] = 0;
    tss[TSS_RSP0_IDX] = (stack_vaddr >> 0) & 0xFFFFFFFF;
    tss[TSS_RSP0_IDX + 1] = (stack_vaddr >> 32) & 0xFFFFFFFF;
    gdt[GDT_TSS_LO] =
        set_gdt_desc((uint32_t)(uintptr_t)&tss, (uint32_t)(sizeof(tss) - 1), 0x89, 0x0);
}

#endif
