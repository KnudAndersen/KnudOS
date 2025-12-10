#ifndef UTILS_C
#define UTILS_C

#include <utils.h>

// TODO: fast memset

void loader_memset(void* a, char ch, u32 n)
{
	u8* p = (u8*)a;
	while (n--) {
		*p++ = ch;
	}
}

#endif
