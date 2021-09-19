CC = g++
CFLAGS = -Wall -g
main: main.o vec.o
	$(CC) $(CFLAGS) -o main main.o vec.o
main.o: main.cpp vec.h
	$(CC) $(CFLAGS) -c main.cpp
	$(CC) $(CFLAGS) -c vec.h
vec.o: vec.h