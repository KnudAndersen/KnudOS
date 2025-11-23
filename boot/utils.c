#ifndef UTILS_C
#define UTILS_C

#include <utils.h>

// TODO: fast memset

void loader_memset(void* a, char ch, U32 n)
{
	U8* p = (U8*)a;
	while (n--) {
		*p++ = ch;
	}
}

#endif
