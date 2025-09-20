#ifndef GSYMS_H
#define GSYMS_H

#include <stdint.h>
#define PAGE_SIZE             (4096)

#define KiB                   (1024ULL)
#define MiB                   (1024 * KiB)
#define GiB                   (1024 * MiB)

#define __BOOT_RESERVE_SIZE__ (25 * 4 * KiB)
#define KSTACK_TOP            (0xFFFFFFFFFFFFF000ULL)
#define KSTACK_BASE           (16 * KiB)
#define KSTACK_MAX            (2 * GiB)

extern const uint64_t __KSTACK_TOP_VADDR__;
extern const uint64_t __KSTACK_BASE_SIZE__;
extern const uint64_t __KHEAP_TOP_VADDR__;

#endif
