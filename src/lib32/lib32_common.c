#ifndef LIB32_COMMON_C
#define LIB32_COMMON_C
#include "lib32_common.h"

uint32_t get_ebx() {
    uint32_t ret;
    asm volatile("mov %0, ebx"
                 : "=m"(ret)
                 :
                 : "ebx");
    return ret;
};

#endif
