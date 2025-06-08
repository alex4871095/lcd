array: main.o libarray.so
	gcc -o arr main.o -L. -larray -Wl,-rpath,. 

main.o: main.c
	gcc main.c -c

libarray.so: sort.o array.o
	gcc -shared -o libarray.so array.o sort.o

array.o: array.c
	gcc array.c -c -fPIC

sort.o: sort.c
	gcc sort.c -c -fPIC

