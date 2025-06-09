all: historic.o memlist.o p2.c
	gcc $(CFLAGS) -o p2 p2.c *.o
	
historic.o: historic.c historic.h
	gcc $(CFLAGS) -c historic.c
	
memlist.o: memlist.c memlist.h
	gcc $(CFLAGS) -c memlist.c
