#ifndef		__ELF_H
#define		__ELF_H

#include <elf.h>
#include <link.h>

#define ELF_MAGIC		"\x7f\x45\x4c\x46"
#define ELF_MAGIC_LEN	4

typedef struct elf {
	uint8_t		*rawp;
	ElfW(Ehdr)	*ehdr;
	ElfW(Shdr)	*shdr;
	char		*shstrp;
} elf_t;

typedef struct memchunk {
	uint8_t		*data;
	size_t		size;
} memchunk_t;

extern void error(char *,...);

elf_t *open_elf(char *path);
void close_elf(elf_t *);
ElfW(Shdr) *elf_get_section_by_name(elf_t *elf, char *name);
memchunk_t elf_get_section_data(elf_t *elf, char *name);

#endif
