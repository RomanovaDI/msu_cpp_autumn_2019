CC = g++

all: run

sum_by_rows:
	$(CC) sum_by_rows.cpp -o sum_by_rows.o

sum_by_columns:
	$(CC) sum_by_columns.cpp -o sum_by_columns.o

run: sum_by_rows sum_by_columns
	./sum_by_rows.o
	./sum_by_columns.o

clean:
	rm *.o