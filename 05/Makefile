CC = g++
FLAGS = -std=c++11

all: test

test: test.out
	./test.out

test.out: test.cpp serializer.h
	$(CC) -o test.out test.cpp $(FLAGS)

clean:
	rm *.o *.out
