CXX=g++
CFLAGS=-Wall -g -std=c++11

all: scheduler triangle_area

scheduler: scheduler.cpp
	$(CXX) $(CFLAGS) -o scheduler scheduler.cpp

triangle_area: triangle_area.cpp
	$(CXX) $(CFLAGS) -o triangle_area triangle_area.cpp

clean:
	rm scheduler triangle_area
