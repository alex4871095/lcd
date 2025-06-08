#hello example makefile
hello: main.o hello.o
	gcc main.o hello.o -o hello

main.o: main.c
	gcc main.c -c
hello.o: hello.c
	gcc hello.c -c

