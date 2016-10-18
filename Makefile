try: tt2.o 
	cc tt2.o -lncurses -o try
tt2.o: tt2.c
	cc -Wall -c tt2.c

