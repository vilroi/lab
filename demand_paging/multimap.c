#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <err.h>
#include <assert.h>

#define NPAGES			5
#define	PAGESIZE 		0x1000

void get_newline();
void alloc_pages(void **, int);

int
main(void)
{
	void		*maps[NPAGES];
	char		*p;
	int			ch;

	printf("pid: %ld\n", (long) getpid());
	get_newline();

	alloc_pages(maps, NPAGES);

	for (int i = 0; i < NPAGES; i++) {
		get_newline();

		p = maps[i];
		*p = 'a';
		printf("data written to %p\n", p);
	}
	get_newline();

	exit(EXIT_SUCCESS);
}

void
alloc_pages(void **arr, int size)
{
	void		*p;

	assert(arr != NULL);

	for (int i = 0; i < size; i++) {
		p = mmap(0, PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC, 
					MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (p == (void *) -1)
			err(1, "mmap failed");
		arr[i] = p;

		printf("Allocated page at %p\n", p);
	}
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
