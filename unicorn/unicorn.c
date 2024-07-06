#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unicorn/unicorn.h>

#define LOAD_ADDRESS		0x10000
#define MEMORY_SIZE			(2 * 1024 * 1024)

extern char *__progname;

void usage(void);
void error(char *,...);
size_t read_all(char *, uint8_t **);
void emulate(uint8_t *, size_t);
void hook_code(uc_engine *, uint64_t, uint32_t, void *);

int
main(int argc, char *argv[])
{
	uint8_t		*insbuf = NULL;
	size_t		nread;

	if (argc != 2)
		usage();

	nread = read_all(argv[1], &insbuf);
	emulate(insbuf, nread);

	exit(EXIT_SUCCESS);
}

void 
emulate(uint8_t *insbuf, size_t len)
{
	uc_engine	*uc;
	uc_err		err;
	size_t		sp = 0x1234;
	uc_hook		trace;

	err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc);
	if (err) 
		error("uc_open failed: %u (%s)\n", err, uc_strerror(err));

	uc_mem_map(uc, LOAD_ADDRESS, MEMORY_SIZE, UC_PROT_ALL);

	uc_mem_write(uc, LOAD_ADDRESS, insbuf, len - 1);

	uc_reg_write(uc, UC_ARM_REG_SP, &sp);

	uc_hook_add(uc, &trace, UC_HOOK_CODE, hook_code, NULL, LOAD_ADDRESS, LOAD_ADDRESS + len - 1);

	err = uc_emu_start(uc, LOAD_ADDRESS | 1, LOAD_ADDRESS + len, 0, 0);
	if (err)
		error("uc_emu_start failed: %u (%s)\n", err, uc_strerror(err));
}

void 
hook_code(uc_engine *uc, uint64_t addr, uint32_t size, void *user_data)
{
	printf(">>> Tracing instruction at 0x%" PRIx64 ", instruction size = 0x%x\n",
			addr, size);
}

size_t
read_all(char *path, uint8_t **bufp)
{
	int				fd;
	struct stat		statbuf;
	size_t			buflen;
	ssize_t			nread;

	if ((fd = open(path, O_RDONLY)) == -1)
		err(1, "failed to open %s\n", path);

	if (fstat(fd, &statbuf) == -1)
		err(1, "fstat failed");

	buflen = statbuf.st_size;

	*bufp = calloc(buflen, sizeof(uint8_t));
	if (*bufp == NULL)
		err(1, "calloc failed");

	if ((nread = read(fd, *bufp, buflen)) < 0)
		err(1, "read failed");

	close(fd);

	return nread;
}


void 
usage(void)
{
	fprintf(stderr, "usage: %s file.bin\n", __progname);
	fprintf(stderr, "	file.bin: a binary file containing arm instructions\n");
	exit(EXIT_FAILURE);
}

__attribute__((noreturn)) void
error(char *fmt,...)
{
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}
