CXX=g++
CFLAGS=-Wall -g -O3

test_search: search.o test_search.o key.o sequence.o
	$(CXX) $(CFLAGS) -o test_search test_search.o search.o key.o sequence.o

test_search.o: test_search.cpp
	$(CXX) $(CFLAGS) -c test_search.cpp

search.o: search.cpp search.h
	$(CXX) $(CFLAGS) -c search.cpp

key.o: key.cpp key.h
	$(CXX) $(CFLAGS) -c key.cpp

sequence.o: sequence.cpp sequence.h
	$(CXX) $(CFLAGS) -c sequence.cpp

test_key: key.o test_key.o
	$(CXX) $(CFLAGS) -o test_key key.o test_key.o
	./test_key

test_key.o: test_key.cpp key.h
	$(CXX) $(CFLAGS) -c test_key.cpp

clean:
	rm *.o test_search test_key
