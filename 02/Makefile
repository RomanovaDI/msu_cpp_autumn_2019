CC = g++
FLAGS = -std=c++11

all: test

test: test.out
	./test.out

test.out: tests.o Allocator.o
	$(CC) -o test.out tests.o Allocator.o $(FLAGS)

Test.o: tests.cpp Allocator.h Timer.h
	$(CC) -c tests.cpp $(FLAGS)

Allocator.o: Allocator.cpp Allocator.h Timer.h
	$(CC) -c Allocator.cpp $(FLAGS)

clean:
	rm *.o *.out
