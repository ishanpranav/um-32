CC = clang
CFLAGS =-pedantic -Wall -Wextra
CAPP = -lum -L. -Wl,-rpath=.
COBJ = -fPIC -c

all: umasm umdasm umvm

um: machine
	$(CC) $(CFLAGS) *.o -o libum.so -shared

umasm: umasm.c um
	$(CC) $(CFLAGS) $(CAPP) umasm.c -o umasm

umdasm: umdasm.c um
	$(CC) $(CFLAGS) $(CAPP) umdasm.c -o umdasm

umvm: umvm.c um
	$(CC) $(CFLAGS) $(CAPP) umvm.c -o umvm

machine: machine.h machine.c fault heap instruction opcode segment \
	reader.h writer.h
	$(CC) $(CFLAGS) $(COBJ) machine.c

fault: fault.h fault.c
	$(CC) $(CFLAGS) $(COBJ) fault.c

heap: heap.h heap.c
	$(CC) $(CFLAGS) $(COBJ) heap.c

instruction: instruction.h instruction.c
	$(CC) $(CFLAGS) $(COBJ) instruction.c

opcode: opcode.h opcode.c
	$(CC) $(CFLAGS) $(COBJ) opcode.c

segment: segment.h segment.c
	$(CC) $(CFLAGS) $(COBJ) segment.c

clean:
	rm -rf *.o *.so umasm umdasm umvm
