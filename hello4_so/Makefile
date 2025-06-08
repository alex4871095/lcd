#hello example makefile
hello: main.o libhello.so
	gcc -o hello main.o -L. -lhello -Wl,-rpath,. 

main.o: main.c
	gcc main.c -c

libhello.so: hello.o goodbye.o
	gcc -shared -o libhello.so hello.o goodbye.o 
hello.o: hello.c
	gcc hello.c -c -fPIC

goodbye.o: goodbye.c
	gcc goodbye.c -c -fPIC

clean:
	rm -f *.o a hello
