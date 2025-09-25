#ifndef KCOMMON_H
#define KCOMMON_H

#include "../../../common/include/multiboot.h"
#include <stdint.h>

#define PAGE_SIZE (4096ULL)

#define KiB       (1024ULL)
#define MiB       (1024ULL * KiB)
#define GiB       (1024ULL * MiB)
#define NULL      ((void*)0)

#define __KERNEL_PANIC__         \
    ({                           \
        while (1) {              \
            asm volatile("hlt"); \
        }                        \
    })
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))

extern const char HEXDIGITS[];
extern const char boot_module_magic[];

#define EBX (0)
#define ESI (1)
#define EDI (2)
uint32_t pop_reg32(uint8_t reg);
int32_t StrCmp(const char* s_1, const char* s_2);
int32_t StrNCmp(const char* s_1, const char* s_2, uint32_t n);
multiboot_mod* get_kernel(multiboot_info* m_info);
void MemSet64(void* ptr, unsigned char byte, uint64_t n);
#endif
