OBJFILES=modinfo.o elf.o
CFLAGS=-g -Wall -Wpedantic

all: modinfo

modinfo: $(OBJFILES)
	$(CC) $(OBJFILES) -o modinfo

modinfo.o: modinfo.c
	$(CC) $(CFLAGS) -c modinfo.c

elf.o: elf.c
	$(CC) $(CFLAGS) -c elf.c

clean:
	rm *.o
	rm modinfo

.PHONY: clean
