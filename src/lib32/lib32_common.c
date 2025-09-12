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

int32_t Strcmp(const char* s_1, const char* s_2) {
    int32_t cmp;
    while (*s_1 && *s_2) {
        cmp = *(s_1++) - *(s_2++);
        if (cmp)
            return cmp;
    }
    return *s_1 - *s_2;
}

int32_t Strcmp_N(const char* s_1, const char* s_2, uint32_t n) {
    if (n == 0)
        return Strcmp(s_1, s_2);
    int32_t cmp;
    for (uint32_t i = 0; i < n; i++) {
        cmp = *(s_1++) - *(s_2++);
        if (cmp)
            return cmp;
    }
    return cmp;
}

#endif
