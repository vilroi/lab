#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

#include "elfparse.h"

extern void error(char *,...);

elf_t *
open_elf(char *path)
{
	int			fd;
	elf_t		*elf;
	struct stat	statbuf;

	if ((elf = calloc(1, sizeof(elf_t))) == NULL)
		err(1, "calloc failed");

	if ((fd = open(path, O_RDONLY)) == -1)
		err(1, "open failed");

	if (fstat(fd, &statbuf) == -1)
		err(1, "fstat failed");

	elf->len = statbuf.st_size;
	elf->rawp = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if ((void *) elf->rawp == MAP_FAILED)
		err(1, "mmap failed");

	if (memcmp(elf->rawp, ELF_MAGIC, ELF_MAGIC_LEN) != 0)
		error("%s not an elf file", path);

	elf->ehdr = (ElfW(Ehdr) *) elf->rawp;
	elf->shdr = (ElfW(Shdr) *) (elf->rawp + elf->ehdr->e_shoff);
	elf->shstrp = elf->rawp + (elf->shdr[elf->ehdr->e_shstrndx].sh_offset) + 1;

	return elf;
}

void 
close_elf(elf_t *elf)
{
	if (elf == NULL)
		return;

	if (munmap(elf->rawp, elf->len) == -1)
		err(1, "munmap failed");

	free(elf);
}

ElfW(Shdr) *
elf_get_section_by_name(elf_t *elf, char *name)
{
	char		*shname;
	ElfW(Shdr)	*shdrp;

	for (int i = 0; i < elf->ehdr->e_shnum; i++) {
		shdrp = elf->shdr + i;
		shname = elf->shstrp + shdrp->sh_name; 

		if (strncmp(shname, name, strlen(shname)) == 0) 
			return shdrp;
	}
	return NULL;
}

memchunk_t
elf_get_section_data(elf_t *elf, char *name)
{
	ElfW(Shdr)		*shdrp;
	memchunk_t		chunk = {0};

	shdrp = elf_get_section_by_name(elf, name);
	if (shdrp == NULL)
		return chunk;

	chunk.data = elf->rawp + shdrp->sh_offset;
	chunk.size = shdrp->sh_size;

	return chunk;
}
