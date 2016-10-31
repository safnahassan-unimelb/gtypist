project: gtypist.o 
	cc gtypist.o -lncurses -o project
gtypist.o: gtypist.c gtypist.h
	cc -Wall -c gtypist.c
clean: rm *.o

