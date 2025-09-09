#ifndef GDT_32_H
#define GDT_32_H

#include "lib32_common.h"

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
#define TSS_NUM_ENT  (26)
#define TSS_RSP0_IDX (1)

typedef struct gdt32_base_reg {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr32_t;

typedef uint64_t gdt_desc;

gdt_desc set_gdt_desc(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
void init_gdt();
void init_tss(uint64_t stack_vaddr);
#endif
