CXX=g++
CFLAGS=-Wall -g -O3 -std=c++11

test: test.o
	$(CXX) $(CFLAGS) -o test test.o

test.o: rand_bst.hpp test.cpp
	$(CXX) $(CFLAGS) -c test.cpp

clean:
	rm *.o test
