#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <err.h>

#include "elfparse.h"

__attribute__((noreturn)) void error(char *,...);
void do_modinfo(char *);

int
main(int argc, char *argv[])
{
	if (argc < 2) 
		error("usage: %s module1.ko module2.ko ...", *argv);

	while (*++argv) {
		printf("%s:\n", *argv);
		do_modinfo(*argv);
		puts("");
	}

	exit(EXIT_SUCCESS);
}

void 
do_modinfo(char *module)
{
	size_t		offset = 0;
	char		*p;
	elf_t		*elf;
	memchunk_t	chunk;

	elf = open_elf(module);
	chunk = elf_get_section_data(elf, "modinfo");
	if (chunk.data == NULL)
		error("modinfo not found");

	p = chunk.data;
	while (offset != chunk.size) {
		puts(p);

		while (*p != '\0') {
			p++;
			offset++;
		}
		p++;
		offset++;
	}

	close_elf(elf);
}
__attribute__((noreturn)) 
void error(char *fmt,...)
{
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}
