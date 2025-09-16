#ifndef COMMON_32_H
#define COMMON_32_H

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

/* compare first n bytes of two strings.
 * If n is 0, behavior is strcmp(s_1, s_2);
 * Otherwise: returns the ASCII difference between n bytes of the strings
 * Note: there is undefined behavior if n > strlen(s_1) && n > strlen(s_2)*/
int32_t StrNCmp(const char* s_1, const char* s_2, uint32_t n);

/* Returns ascii difference between two strings, terminated by '\0' */
int32_t StrCmp(const char* s_1, const char* s_2);

#endif
