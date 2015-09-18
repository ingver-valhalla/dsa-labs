CXX=g++
CFLAGS=-Wall -g
OBJ=gen_seq.o sequence.o

gen_seq: $(OBJ)
	$(CXX) -o gen_seq $(OBJ)

gen_seq.o: gen_seq.cpp
	$(CXX) $(CFLAGS) -c $*.cpp

sequence.o: sequence.cpp sequence.hpp
	$(CXX) $(CFLAGS) -c $*.cpp

make_coords: $@.cpp
	$(CXX) $@.cpp -o $@

clean:
	rm make_coords gen_seq *.o
