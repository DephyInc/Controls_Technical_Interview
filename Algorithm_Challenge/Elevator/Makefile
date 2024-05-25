CC=gcc
CFLAGS=-I.

# Target to create the executable
elevator_problem: elevator_problem.o
	$(CC) -o elevator_problem elevator_problem.o

# Rule to compile the object file
elevator_problem.o: elevator_problem.c elevator_problem.h
	$(CC) -c -o elevator_problem.o elevator_problem.c $(CFLAGS)

run: elevator_problem
	./elevator_problem

install: run

clean:
	rm -f elevator_problem elevator_problem.o
