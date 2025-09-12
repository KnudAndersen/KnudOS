#ifndef LIB32_COMMON_H
#define LIB32_COMMON_H

#include <stdint.h>

#define KiB  (1024ULL)
#define MiB  (1024ULL * KiB)
#define GiB  (1024ULL * MiB)

#define NULL (0ULL)

#define __KERNEL_PANIC__         \
    ({                           \
        while (1) {              \
            asm volatile("hlt"); \
        }                        \
    })
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))
uint32_t get_ebx();
#endif
