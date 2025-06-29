all: threads
threads:
	gcc threads.c -D_REENTERANT -o threads  -lpthread
	strip threads
clean:
	 rm -f *.o   
	 rm -f threads
	 rm -f *~
