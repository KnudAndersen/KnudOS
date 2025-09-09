#include <stdint.h>

#define LOAD_ERR            (1)
#define LOAD_SUCC           (0)

#define GDT_NUM_ENT         (4 + 2) // + tss
#define GDT_ACC_PRES        (0 << 7)
#define GDT_ACC_USER        (3 << 5)
#define GDT_ACC_CODE_DATA   (1 << 4)
#define GDT_ACC_EXEC        (1 << 3)
#define GDT_ACC_RDWR        (1 << 1)
#define GDT_FLAGS_GRAN      (1 << 3)
#define GDT_FLAGS_LONG_MODE (1 << 1)
#define GDT_LIM_LO_MASK     (0xFFFF)
#define GDT_LIM_HI_MASK     (0xF)
#define GDT_LIM_LO_SHIFT    (0)
#define GDT_LIM_HI_SHIFT    (48)
#define GDT_BASE_LO_MASK    (0xFFFFFF)
#define GDT_BASE_HI_MASK    (0xFF)
#define GDT_BASE_LO_SHIFT   (16)
#define GDT_BASE_HI_SHIFT   (56)
#define GDT_ACCESS_SHIFT    (40)
#define GDT_FLAGS_MASK      (0xF)
#define GDT_FLAGS_SHIFT     (52)
#define GDT_DEFAULT_ACC     (GDT_ACC_PRES | GDT_ACC_CODE_DATA | GDT_ACC_RDWR)
#define GDT_DEFAULT_FLAG    (GDT_FLAGS_GRAN | GDT_FLAGS_LONG_MODE)
#define GDT_KERN_CODE_ACC   (GDT_DEFAULT_ACC | GDT_ACC_EXEC)
#define GDT_KERN_DATA_ACC   (GDT_DEFAULT_ACC)
#define GDT_USER_CODE_ACC   (GDT_DEFAULT_ACC | GDT_ACC_EXEC | GDT_ACC_USER)
#define GDT_USER_DATA_ACC   (GDT_DEFAULT_ACC | GDT_ACC_USER)
enum { GDT_NULL = 0, GDT_KERN_CODE, GDT_KERN_DATA, GDT_USER_CODE, GDT_USER_DATA, GDT_TSS_LO };

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

#define TSS_NUM_ENT         (26)
#define TSS_RSP0_IDX        (1)

typedef struct kernel_long_jump {
    uint32_t entry;
    uint16_t selector;
} __attribute__((packed)) ljmp_t;

typedef struct gdt32_base_reg {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr32_t;

typedef struct {
    uint64_t ents[PG_ENT];
} __attribute__((packed)) page_t;

typedef uint64_t gdt_desc;

extern uint32_t __loader_end__;

gdt_desc gdt[GDT_NUM_ENT];
uint32_t tss[TSS_NUM_ENT];
gdtr32_t gdtr;
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
