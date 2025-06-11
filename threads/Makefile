all: canceltest canceltest2
canceltest:
	gcc canceltest.c -D_REENTERANT -o canceltest  -lpthread
	strip canceltest
canceltest2:
	gcc canceltest2.c -D_REENTERANT -o canceltest2  -lpthread
	strip canceltest2
clean:
	 rm -f *.o   
	 rm -f canceltest canceltest2
	 rm -f *~
