all: historic.o memlist.o proclist.o p3.c
	gcc $(CFLAGS) -o p3 p3.c *.o
	
historic.o: historic.c historic.h
	gcc $(CFLAGS) -c historic.c
	
memlist.o: memlist.c memlist.h
	gcc $(CFLAGS) -c memlist.c
	
proclist.o: proclist.c proclist.h
	gcc $(CFLAGS) -c proclist.c
