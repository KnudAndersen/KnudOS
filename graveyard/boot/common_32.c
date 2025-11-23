#ifndef COMMON_32_C
#define COMMON_32_C

#include "./include/common_32.h"

uint32_t get_ebx() {
    uint32_t ret;
    asm volatile("mov %0, ebx"
                 : "=m"(ret)
                 :
                 : "ebx");
    return ret;
};

int32_t StrCmp(const char* s_1, const char* s_2) {
    int32_t cmp;
    while (*s_1 && *s_2) {
        cmp = *(s_1++) - *(s_2++);
        if (cmp)
            return cmp;
    }
    return *s_1 - *s_2;
}

int32_t StrNCmp(const char* s_1, const char* s_2, uint32_t n) {
    if (n == 0)
        return StrCmp(s_1, s_2);
    int32_t cmp;
    for (uint32_t i = 0; i < n; i++) {
        cmp = *(s_1++) - *(s_2++);
        if (cmp)
            return cmp;
    }
    return cmp;
}

#endif
