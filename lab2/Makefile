CXX=g++
CFLAGS=-Wall -g -O3
LAB1=../lab1

test_sorts: sequence.o test_sorts.o
	$(CXX) $(CFLAGS) -o test_sorts test_sorts.o sequence.o

test_sorts.o: sequence.o test_sorts.cpp sorts.hpp
	$(CXX) $(CFLAGS) -c test_sorts.cpp 

sequence.o: $(LAB1)/sequence.hpp $(LAB1)/sequence.cpp 
	$(CXX) $(CFLAGS) -c $(LAB1)/sequence.cpp

clean:
	rm *.o test_sorts
