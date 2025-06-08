#hello example makefile
hello: main.o libhello.a
	gcc -o hello main.o -L. -lhello

main.o: main.c
	gcc main.c -c

libhello.a: hello.o goodbye.o
	ar cr libhello.a hello.o goodbye.o

hello.o: hello.c
	gcc hello.c -c

goodbye.o: goodbye.c
	gcc goodbye.c -c

clean:
	rm -f *.o a hello
