all: lab6

lab6: lab6.o
	gcc lab6.o -lpthread -o lab6
lab6.o: lab6.c
	gcc -c lab6.c -lpthread
clean:
	rm -f *.o lab6
