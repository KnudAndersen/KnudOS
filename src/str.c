#include <str.h>

size_t strlen(char* str)
{
	char* tmp = str;
	while (*tmp)
		tmp++;
	return (size_t)(tmp - str);
}

int strcmp(const char* s1, const char* s2)
{
	int sum = 0;
	for (int i = 0; s1[i] && s2[i]; i++) {
		sum += (s1[i] - s2[i]);
	}
	return sum;
}

// TODO: perform 8-byte aligned long writes

void memcpy(void* dst, const void* src, size_t n)
{
	const char* src_p = src;
	char* dst_p = dst;
	while (n && n--)
		*(dst_p++) = *(src_p++);
}

void memset(void* ptr, char byte, size_t n)
{
	char* p = ptr;
	while (n && n--)
		*(p++) = byte;
}

// Volatile-friendly "overloads"
// TODO: implement _Generics or something better
void memcpy_v(volatile void* dst, const void* src, size_t n)
{
	const volatile char* src_p = src;
	volatile char* dst_p = dst;
	while (n && n--)
		*(dst_p++) = *(src_p++);
}

void memset_v(volatile void* ptr, char byte, size_t n)
{
	volatile char* p = ptr;
	while (n && n--)
		*(p++) = byte;
}
