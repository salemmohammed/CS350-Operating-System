CC = gcc
CFLAGS = -std=c11 -Wall -pedantic

OBJECTS = lab4.o 
OBJECTS5 = lab5.o

all:	lab4 lab5

lab4:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS)  -o $@

lab5:	$(OBJECTS5)
	$(CC) $(CFLAGS) $(OBJECTS5)  -o $@

lab4.o:	lab4.c
lab5.o:	lab5.c

clean:
	rm -f *.o lab4 lab5

