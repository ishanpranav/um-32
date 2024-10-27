CC = clang
CFLAGS =-pedantic -Wall -Wextra
CEXTRAS = -lum -L. -Wl,-rpath=.

all: umasm umdasm umvm

um: machine
	$(CC) $(CFLAGS) *.o -o libum.so -shared

umasm: umasm.c um
	$(CC) $(CFLAGS) $(CEXTRAS) umasm.c -o umasm

umdasm: umdasm.c um
	$(CC) $(CFLAGS) $(CEXTRAS) umdasm.c -o umdasm

umvm: umvm.c um
	$(CC) $(CFLAGS) $(CEXTRAS) umvm.c -o umvm

machine: machine.h machine.c fault opcode segment reader.h writer.h
	$(CC) $(CFLAGS) -fPIC -c machine.c

fault: fault.h fault.c
	$(CC) $(CFLAGS) -fPIC -c fault.c

opcode: opcode.h opcode.c
	$(CC) $(CFLAGS) -fPIC -c opcode.c

segment: segment.h segment.c
	$(CC) $(CFLAGS) -fPIC -c segment.c

clean:
	rm -rf *.o *.so umasm umdasm umvm
