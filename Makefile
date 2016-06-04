# makefile for EDA final project 
# floorplanner
CXX = g++
CXXFLAGS =
OBJ = 

.PHONY: all clean

all: main

main: main.o
	$(CXX) $(CXXFLAGS) main.o -o router

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf *.o

