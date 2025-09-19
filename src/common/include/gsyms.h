#ifndef GSYMS_H
#define GSYMS_H

#include <stdint.h>

#define KiB                   (1024ULL)

#define __BOOT_RESERVE_SIZE__ (40 * 4 * KiB)

extern const uint64_t __KSTACK_TOP_VADDR__;
extern const uint64_t __KSTACK_BASE_SIZE__;

#endif
