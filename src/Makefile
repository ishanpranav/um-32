CC = clang
CFLAGS =-pedantic -Wall -Wextra

umasm: umasm.c machine
	$(CC) $(CFLAGS) umasm.c *.o -o umasm

umdasm: umdasm.c machine
	$(CC) $(CFLAGS) umdasm.c *.o -o umdasm

machine: machine.h machine.c opcode segment reader.h writer.h
	$(CC) $(CFLAGS) -c machine.c

opcode: opcode.h opcode.c
	$(CC) $(CFLAGS) -c opcode.c

segment: segment.h segment.c
	$(CC) $(CFLAGS) -c segment.c

clean:
	rm -rf *.o umasm umdasm umvm
