all: shmemdemo
shmemdemo:
	gcc shmemserv.c -o shmemserv
	gcc shmemcli.c -o shmemcli
clean:
	 rm -f *.o   
	 rm -f *serv *cli
	 rm -f *~
