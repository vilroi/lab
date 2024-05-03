CC=gcc
CFLAGS=-Wall -Wpedantic 
#LDFLAGS=-static
OBJFILES=main.o error.o packet.o utils.o

icmp_server: $(OBJFILES)
	$(CC) $(LDFLAGS) -o icmp_server $(OBJFILES)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

error.o: error.c
	$(CC) -c $(CFLAGS) error.c -o error.o

utils.o: utils.c
	$(CC) -c $(CFLAGS) utils.c -o utils.o

packet.o: packet.c
	$(CC) -c $(CFLAGS) packet.c -o packet.o

clean:
	rm icmp_server *.o

.PHONY: clean
