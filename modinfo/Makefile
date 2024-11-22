OBJFILES=modinfo.o elf.o

all: modinfo

modinfo: $(OBJFILES)
	$(CC) $(OBJFILES) -o modinfo

modinfo.o: modinfo.c
	$(CC) -c modinfo.c

elf.o: elf.c
	$(CC) -c elf.c

clean:
	rm *.o
	rm modinfo

.PHONY: clean
