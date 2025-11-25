#include <str.h>

size_t StrLen(const char* str)
{
	const char* tmp = str;
	while (*tmp)
		tmp++;
	return (size_t)(tmp - str);
}
