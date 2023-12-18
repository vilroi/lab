all: singlemap multimap  ptdump

singlemap: singlemap.c
	$(CC) singlemap.c -o singlemap

multimap: multimap.c
	$(CC) multimap.c -o multimap

ptdump: true_flag
	$(MAKE) -C $@

clean:
	rm singlemap multimap ptdump/ptdump

true_flag: 
	true

.PHONY: clean true
