CXX=g++
CFLAGS=-Wall -g -O3

main: string_search.o main.o
	$(CXX) $(CFLAGS) -o main main.o string_search.o

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

string_search.o: string_search.h string_search.cpp
	$(CXX) $(CFLAGS) -c string_search.cpp

clean:
	rm *.o main
