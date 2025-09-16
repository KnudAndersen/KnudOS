#ifndef KCOMMON_H
#define KCOMMON_H

#include <stdint.h>

#define PAGE_SIZE (4096ULL)

#define KiB       (1024ULL)
#define MiB       (1024ULL * KiB)
#define GiB       (1024ULL * MiB)
#define NULLPTR   ((void*)0)
#define NULLVAL   ((void)0)
#define __KERNEL_PANIC__         \
    ({                           \
        while (1) {              \
            asm volatile("hlt"); \
        }                        \
    })
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))

extern const char HEXDIGITS[];
#endif
