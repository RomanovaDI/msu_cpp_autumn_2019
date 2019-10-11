CC = g++
FLAGS = -std=c++11

all: test

test: calc
	python3 ./test.py

calc: calc.cpp
	$(CC) -o calc calc.cpp $(FLAGS)

clean:
	rm calc