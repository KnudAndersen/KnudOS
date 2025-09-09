#ifndef LOAD_32_C
#define LOAD_32_C

#include "gdt.h"
#include <stdint.h>

#define LOAD_ERR            (1)
#define LOAD_SUCC           (0)

#define PAGE_SIZE           (4096)
#define PG_ENT              (512)
#define PG_PAGE_OFF_MASK    (0xFFF)
#define PG_STRUC_IDX_MASK   (0x1FF)
#define PG_STRUC_STATIC_NUM (256 + 1 + 1)
#define PG_PRESENT          (1 << 0)
#define PG_RDWR             (1 << 1)
#define PG_DEFAULT_FLAGS    (PG_PRESENT | PG_RDWR)

#define CR0_PG              (1ULL << 31)
#define CR4_PAE             (1ULL << 5)
#define IA32_EFER_ADDR      (0xC0000080)
#define IA32_EFER_LME       (1 << 8)

typedef struct kernel_long_jump {
    uint32_t entry;
    uint16_t selector;
} __attribute__((packed)) ljmp_t;

typedef struct {
    uint64_t ents[PG_ENT];
} __attribute__((packed)) page_t;

extern uint32_t __loader_end__;

page_t pml4;
page_t pg_reserve[PG_STRUC_STATIC_NUM];
int reserve_used = 0;

uint32_t get_ebx() {
    uint32_t ret;
    asm volatile("mov %0, ebx"
                 : "=m"(ret)
                 :
                 : "ebx");
    return ret;
};
uint32_t long_mode_supported() {
    return LOAD_SUCC;
};
extern gdtr32_t gdtr;

void init_cpu_state() {
    asm volatile("lgdt [%0]"
                 :
                 : "r"(&gdtr)
                 :);
    asm volatile("mov ax, %0\n\t"
                 "ltr ax"
                 :
                 : "r"((uint16_t)(GDT_TSS_LO * sizeof(uint64_t)))
                 : "ax");
    asm volatile("mov eax, cr0\n\t"
                 "and eax, %0\n\t"
                 "mov cr0, eax"
                 :
                 : "r"((uint32_t)(0xFFFFFFFF & ~(CR0_PG)))
                 : "eax");
    asm volatile("mov eax, cr4\n\t"
                 "or eax, %0\n\t"
                 "mov cr4, eax"
                 :
                 : "r"((uint32_t)CR4_PAE)
                 : "eax");
    asm volatile("mov cr3, %0"
                 :
                 : "r"(&pml4)
                 :);
    asm volatile("mov ecx, %0\n\t"
                 "rdmsr\n\t"
                 "or eax, %1\n\t"
                 "xor edx, edx\n\t"
                 "wrmsr"
                 :
                 : "r"((uint32_t)IA32_EFER_ADDR), "r"((uint32_t)IA32_EFER_LME)
                 : "eax", "ebx", "ecx");
    /* CR0.PG=1*/
    asm volatile("mov eax, cr0\n\t"
                 "or eax, %0\n\t"
                 "mov cr0, eax"
                 :
                 : "r"((uint32_t)CR0_PG)
                 : "eax");
    return;
}

uint32_t load_kernel(void* multiboot) {
    return LOAD_ERR;
}

uint32_t loader_main(uint32_t boot_stack) {
    if (long_mode_supported() == LOAD_ERR) {
        return LOAD_ERR;
    }
    void* mboot_paddr = (void*)(uintptr_t)get_ebx();
    init_gdt();
    init_tss(boot_stack);

    for (int i = 0; i < PG_ENT; ++i)
        pml4.ents[i] = 0;
    for (int i = 0; i < PG_STRUC_STATIC_NUM; ++i) {
        for (int j = 0; j < PG_ENT; ++j)
            pg_reserve[i].ents[j] = 0;
    }

    asm volatile("hlt");
    init_cpu_state();
    while (1)
        asm volatile("hlt");

    if (load_kernel(mboot_paddr) == LOAD_ERR) {
        return LOAD_ERR;
    }
    return 0;
}

#endif
