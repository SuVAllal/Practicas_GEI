all: historic.o p1.c
	gcc $(CFLAGS) -o p1 p1.c *.o
	
historic.o: historic.c historic.h
	gcc $(CFLAGS) -c historic.c
	
	
valgrind: all p1
	valgrind --leak-check=full --show-reachable=yes ./p1
