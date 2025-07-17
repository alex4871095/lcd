CC=gcc

all: hello.c
	$(CC) `pkg-config --cflags gtk4` hello.c -o hello `pkg-config --libs gtk4`

clean:
	/bin/rm -f *.o *~