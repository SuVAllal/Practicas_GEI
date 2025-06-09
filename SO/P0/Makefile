all: historial.o p0.c
	gcc $(CFLAGS) -o p0 p0.c *.o
	
historic.o: historial.c historial.h
	gcc $(CFLAGS) -c historial.c
	
	
valgrind: all p0
	valgrind --leak-check=full --show-reachable=yes ./p0
