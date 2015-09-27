CXX=g++
CFLAGS=-Wall -g
LAB1=../lab1

sorts: sorts.cpp sequence.o
	$(CXX) $(CFLAGS) -o sorts sorts.cpp sequence.o

sequence.o: $(LAB1)/sequence.cpp 
	$(CXX) $(CFLAGS) -c $(LAB1)/sequence.cpp

clean:
	rm *.o sorts
