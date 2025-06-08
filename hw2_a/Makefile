array: main.o libarray.a
	gcc -o arr main.o -L. -larray

main.o: main.c
	gcc main.c -c

libarray.a: array.o sort.o
	ar cr libarray.a array.o sort.o

array.o: array.c
	gcc array.c -c

sort.o: sort.c
	gcc sort.c -c

