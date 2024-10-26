CC = clang
CFLAGS =-pedantic -Wall -Wextra

main: main.c byte.h machine.h register.h word.h
	$(CC) $(CFLAGS) main.c -o um32

clean:
	rm -rf *.o um32
