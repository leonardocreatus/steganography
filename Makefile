CC      = gcc
CFLAGS  = -c -O2 -Wall
LDFLAGS =

all: hide

hide: lib_ppm.o main.o tools.o
	$(CC) $(LDFLAGS) lib_ppm.o main.o tools.o -o hide

lib_ppm.o: lib_ppm.c
	$(CC) $(CFLAGS) -c lib_ppm.c

tools.o: tools.c
	$(CC) $(CFLAGS) -c tools.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	-rm -f *.o *~ main 
