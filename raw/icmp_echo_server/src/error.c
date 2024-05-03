#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void 
error(char *fmt,...)
{
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
}

void 
error_exit(char *fmt,...)
{
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}
