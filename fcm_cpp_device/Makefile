# Flags
CC = g++
CFLAGS = -Wall -g

# File names
SOCKET = socket_rw
A429 = a429_lib
PLANE = plane

# Object files
OBJS = main.o $(SOCKET).o $(A429).o $(PLANE).o
 
# Targets
main: $(OBJS)
    $(CC) $(CFLAGS) -o main $(OBJS)
 
main.o: main.cpp $(SOCKET).h $(A429).h $(PLANE).h
    $(CC) $(CFLAGS) -c main.cpp
 
$(SOCKET).o: $(SOCKET).cpp $(SOCKET).h
    $(CC) $(CFLAGS) -c $(SOCKET).cpp
 
$(A429).o: $(A429).cpp $(A429).h
    $(CC) $(CFLAGS) -c $(A429).cpp

$(PLANE).o: $(PLANE).cpp $(PLANE).h
    $(CC) $(CFLAGS) -c $(PLANE).cpp