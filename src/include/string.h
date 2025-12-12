#ifndef STRING_H
#define STRING_H

#include <types.h>

size_t strlen(char* str);
int strcmp(const char* s1, const char* s2);

void memcpy(void* dst, const void* src, size_t n);
void memset(void* ptr, char byte, size_t n);

void memcpy_v(volatile void* dst, const void* src, size_t n);
void memset_v(volatile void* ptr, char byte, size_t n);

#endif
