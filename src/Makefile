CC = clang
CFLAGS =-pedantic -Wall -Wextra

main: main.c machine
	$(CC) $(CFLAGS) main.c *.o -o um32

machine: machine.h machine.c opcode segment reader.h writer.h
	$(CC) $(CFLAGS) -c machine.c

opcode: opcode.h opcode.c
	$(CC) $(CFLAGS) -c opcode.c

segment: segment.h segment.c
	$(CC) $(CFLAGS) -c segment.c

clean:
	rm -rf *.o um32
