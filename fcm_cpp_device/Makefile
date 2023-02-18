# Flags
CC = g++
CFLAGS = -Wall -g
 
# Targets 
main: main.o Point.o Square.o
    $(CC) $(CFLAGS) -o main main.o Point.o Square.o
 
main.o: main.cpp Point.h Square.h
    $(CC) $(CFLAGS) -c main.cpp
 
Point.o: Point.h
 
Square.o: Square.h Point.h