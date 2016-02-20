CXX=g++
CFLAGS=-Wall -g -O3

main: tree.o main.o
	$(CXX) $(CFLAGS) -o main main.o tree.o

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

tree.o: tree.h tree.cpp
	$(CXX) $(CFLAGS) -c tree.cpp

clean:
	rm *.o main
