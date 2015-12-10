CXX=g++
CFLAGS=-Wall -g -O3

main: main.o
	$(CXX) $(CFLAGS) -o main main.o

main.o: AbstractBST.h RandomizedBST.h RandomizedBST.hpp main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	rm *.o main
