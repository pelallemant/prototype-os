#include <prototype-os/libc/string.h>

void *memcpy(char *dst, const char *src, int n)
{
	char *p = dst;
	while (n--)
		*dst++ = *src++;
	return p;
}

