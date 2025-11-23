#include <str.h>

SIZE_T StrLen(CHAR* str)
{
	CHAR* tmp = str;
	while (*tmp)
		tmp++;
	return (SIZE_T)(tmp - str);
}
