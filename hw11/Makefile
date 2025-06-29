all: mutex3 tictac 
mutex3: 
	gcc mutex3.c -D_REENTERANT -o mutex3  -lpthread -lrt
	strip mutex3
tictac:
	gcc tictac.c -D_REENTERANT -o tictac  -lpthread -lrt
	strip tictac
clean:
	rm -f *.o   
	rm -f mutex3 tictac
	rm -f *~

