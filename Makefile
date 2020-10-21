CC = gcc
CFLAGS = -Wall -std=c99

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) -g memgrind.o mymalloc.o -o memgrind

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c -o memgrind.o

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -g -c mymalloc.c -o mymalloc.o

clean:
	rm -f memgrind *.o