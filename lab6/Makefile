CXX=g++
CFLAGS=-Wall -g -O3

main: main.cpp btree.o
	$(CXX) $(CFLAGS) -o main main.cpp btree.o

btree.o: btree.h btree.cpp
	$(CXX) $(CFLAGS) -c btree.cpp

clean:
	rm *.o main


