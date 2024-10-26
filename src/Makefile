CC = clang
CFLAGS =-pedantic -Wall -Wextra

main: main.c machine
	$(CC) $(CFLAGS) main.c *.o -o um32

machine: machine.h machine.c segment reader.h register.h writer.h
	$(CC) $(CFLAGS) -c machine.c

segment: segment.h segment.c
	$(CC) $(CFLAGS) -c segment.c

clean:
	rm -rf *.o um32
