#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <err.h>

#define	PAGESIZE 		0x1000

void get_newline();

int
main(void)
{
	char		*p;

	printf("pid: %ld\n", (long) getpid());
	get_newline();

	p = mmap(0, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC, 
				MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (p == (void *) -1)
		err(1, "mmap failed");
	printf("Allocated new map: %p\n", p);
	get_newline();

	printf("Writing data to %p\n", p);
	*p = 'a';

	get_newline();

	exit(EXIT_SUCCESS);
}

void 
get_newline()
{
	int			ch;

	while ((ch = getchar()) != EOF) {
		if (ch == '\n')
			break;
	}
}
