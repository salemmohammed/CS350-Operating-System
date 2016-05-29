CC = g++
CPPFLAGS = -std=c++11 -Wall -pedantic
OBJECTS = program2.o Block.o IMap.o INode.o LFS.o Segment.o

all:	program2

program2:	init $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) -o $@

%.o:	%.h	%.cpp
	$(CC) $(CPPFLAGS) -c $?

clean:
	rm -r -f *.o *.gch *.tar.gz DRIVE program2

tar:	clean
	tar -zcvf program2.tar.gz *.cpp *.h README.md

init:
	mkdir -p DRIVE
