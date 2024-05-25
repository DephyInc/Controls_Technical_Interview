CC=gcc
CFLAGS=-I.

# Target to create the executable
elevator_problem: traffic_light.o
	$(CC) -o elevator_problem traffic_light.o

# Rule to compile the object file
traffic_light.o: traffic_light_problem.c traffic_light_problem.h
	$(CC) -c -o traffic_light.o traffic_light_problem.c $(CFLAGS)

run: traffic_light
	./traffic_light

install: run

clean:
	rm -f traffic_light traffic_light.o
