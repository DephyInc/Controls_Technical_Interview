/****************************************************************************
	[Project] Controls Technical Interview
*****************************************************************************
	[Lead developer] Rachel Harris, rharris at dephy dot com.
	[Contributors] 
*****************************************************************************
	[This file] elevator_problem: Moves an elevator through a building to get 
	people to the correct floor in the least amount of time. 
****************************************************************************
	[Created] 2022-11-07 | rharris | Initial creation
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************
#include "elevator_problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************************************************
// Variable(s)
//****************************************************************************
struct building_s myBuilding;

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************
static void initBuilding(void);
static int8_t setNextElevatorStop(struct building_s building);
static void moveElevator(struct elevator_s * elevator);
static void stopElevator(struct building_s * building);
static void drawBuilding(struct building_s building, int8_t door_status);
static void drawFloor(struct floor_s floor, int8_t floorNumber, struct elevator_s elevator, int8_t doorStatus);
static void drawElevator(struct elevator_s elevator, int8_t doorStatus);
static void delay(int16_t ms);

//****************************************************************************
// Functions You (the Interviewee) Should Edit:
//****************************************************************************

//Returns the floor the elevator should STOP at next
//To stop at a floor means to open the doors and let passengers on and off. It 
//is possible to pass through a floor without stopping there.
//Note: The output should be a number between 0 and (BUILDING_HEIGHT-1), inclusive
// enum for the direction of the elevator. 
static struct  elevator_direction_state
{
	enum {INITIAL, UP, DOWN, STOP} direction;
};

static struct elevator_direction_state elevator_direction;

// this is a helper function which returns the number of passengers in the elevator.A
static int8_t num_passengers(struct building_s building)
{
	int8_t num_passengers = 0;
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if (building.elevator.passengers[i] != -1)
		{
			num_passengers++;
		}
	}
	return num_passengers;
}

// the elevator direction or check if the 
static int8_t get_elevator_direction(struct building_s building)
{
	// if the no one is present it the elevator ( all -1 )	then return the initial direction.
	if (num_passengers(building) == 0)
	{
		elevator_direction.direction = INITIAL;
		return elevator_direction.direction;
	}


	// if the elevator current direction is up, and if any 'one' passenger value is greater than current floor go up else go down.
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if ((building.elevator.passengers[i] != -1 ) && (building.elevator.passengers[i] > building.elevator.currentFloor) && (elevator_direction.direction == UP || elevator_direction.direction == INITIAL))
		{
			return elevator_direction.direction = UP;
		}
	}

	// if the elevator current direction is down, and if any 'one' passenger value is less than current floor go down else go up.
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if ((building.elevator.passengers[i] != -1 ) && (building.elevator.passengers[i] < building.elevator.currentFloor) && (elevator_direction.direction == DOWN || elevator_direction.direction == INITIAL))
		{
			return elevator_direction.direction = DOWN;
		}
	}

	// if the code comes here then the direction should be changed based on the closest stop.
	
	int8_t closest_stop = 0;
	int8_t current_def = abs(building.elevator.currentFloor - building.elevator.passengers[closest_stop]);
	if (building.elevator.passengers[0] == -1)
	{
		current_def = 100;
	}
	for (int8_t i = 1; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if ( (building.elevator.passengers[i] != -1) && (abs(building.elevator.passengers[i] - building.elevator.currentFloor) < current_def) )
	{
			closest_stop = i;
		}
	}

	if (building.elevator.passengers[closest_stop] > building.elevator.currentFloor)
	{
		return elevator_direction.direction = UP;
	}
	else
	{
		return elevator_direction.direction = DOWN;
	}
}





static int8_t get_next_stop(struct building_s building, int8_t direction)
{
	// default is the current location of the elevator.
	int8_t next_stop = building.elevator.currentFloor;
	int8_t num_passanger = num_passengers(building);
	if (num_passanger < ELEVATOR_MAX_CAPACITY)
	{
		// if the current floor has departures, then go there.
		if (building.floors[building.elevator.currentFloor].departures[0] != -1 || building.floors[building.elevator.currentFloor].departures[1] != -1)
		{
			return building.elevator.currentFloor;
		}
	}

	// if any of the passengers desitination is here then return the current floor as the stop.
	if (building.elevator.passengers[0] == building.elevator.currentFloor || building.elevator.passengers[1] == building.elevator.currentFloor || building.elevator.passengers[2] == building.elevator.currentFloor)
	{
		return building.elevator.currentFloor;
	}


	// this can be optimized and combined  with change in direction code. But will leave it here for now as its not affecting the performance.

	if (direction == UP)
	{
		int8_t closest_index = 0;
		int8_t current_def = building.elevator.currentFloor - building.elevator.passengers[0];
		if (building.elevator.passengers[0] == -1)
		{
			 current_def = 100;
		}
		for (int8_t i = 1; i < ELEVATOR_MAX_CAPACITY; i++)
		{
			if (building.elevator.passengers[i] > building.elevator.currentFloor)
			{
				if ((building.elevator.passengers[i] != -1) &&  (abs(building.elevator.passengers[i] - building.elevator.currentFloor) < current_def))
				{
					closest_index = i;
				}
			}
		}
		next_stop = building.elevator.passengers[closest_index];
	}
	if (direction == DOWN)
	{
		int8_t closest_index = 0;
		int8_t current_def = building.elevator.passengers[0] - building.elevator.currentFloor;
		if (building.elevator.passengers[0] == -1)
		{
			current_def = 100;
		}
		

		for (int8_t i = 1; i < ELEVATOR_MAX_CAPACITY; i++)
		{
			if (building.elevator.passengers[i] < building.elevator.currentFloor)
			{
				if ((building.elevator.passengers[i] != -1) &&  (building.elevator.currentFloor - building.elevator.passengers[i]) < current_def)
				{
					closest_index = i;
				}
			}
		}
		next_stop = building.elevator.passengers[closest_index];
	}
	
	return next_stop;
}

/*
The Main idea is to:
1. get the direction of the elevator moving in a particular direction. 
2. If there are drops along the way do it.
3. If there are any drop request along the way do it.
4. if full then keep proceeding to the next stop.

Note; The direction change happens when the passengers in the elevator are no longer have the destination along the direction of the travel.

Possible improvements:
1. The code can be optimized by combining the direction change and the next stop code.
2. if you drop off all the passengers then elevator will search for the next floor with the passengers. This step does not account for the closet.
*/
 

static int8_t setNextElevatorStop(struct building_s building)
{

	int8_t direction = get_elevator_direction(building);
	if (direction == INITIAL){
		// if the elevator is on the floor with the people in depature then wait. this will happen in the inital condition.
		if (building.floors[building.elevator.currentFloor].departures[0] != -1 || building.floors[building.elevator.currentFloor].departures[1] != -1)
		{
			return building.elevator.currentFloor;
		}
		// else I landed in the state where the elevator is empty and the floor is empty. 
		// check which floor has the non (-1) number of departures go there.
		for (int8_t i = 0; i < BUILDING_HEIGHT; i++)
		{
			if (building.floors[i].departures[0] != -1 || building.floors[i].departures[1] != -1) // I am hardcoding the 0 and 1. I can use the for loop but I am lazy
			{
				return i;
			}
		}
	}

	// if the not initial then we need to either go up or down.
	int8_t next_stop = get_next_stop(building, direction);
	if (next_stop == -1)
	{
		elevator_direction.direction = INITIAL;
		return 0;
	}
	return next_stop;

	// // check if the elevator has all the passenger -1, then set the direction to the initial.

	// printf("Elevator is on floor %i\n", building.elevator.currentFloor);
	// printf("Departures: %i, %i\n", building.floors[building.elevator.currentFloor].departures[0], building.floors[building.elevator.currentFloor].departures[1]);
	// printf("Arrivals: %i\n", building.floors[building.elevator.currentFloor].arrivals);
	// return 0;
}


//****************************************************************************
//YOU CAN REVIEW THE CODE BELOW BUT DO NOT EDIT IT UNLESS YOU'RE 3000% SURE 
//THERE'S A BUG IN MY CODE! (which admittedly, is possible)
//****************************************************************************


//****************************************************************************
// Public function(s):
//****************************************************************************

//Main function to run the elevator simulation
void main(void)
{
	//Seed my random number generator
	srand(time(0));

	//Initialize the building
	initBuilding();

	//Draw the initial state of the building, with elevator doors closed
	system("clear");
	drawBuilding(myBuilding,1);
	fflush(stdout);
	delay(1000);

	//Run the elevator for a set period of time
	for(int8_t i = 0; i < 60; i++)
	{
		//Choose the next floor to stop at
		myBuilding.elevator.nextStop = setNextElevatorStop(myBuilding);

		//Move the elevator closer to the next stop
		moveElevator(&myBuilding.elevator);

		//Draw the building, with elevator doors closed
		system("clear");
		drawBuilding(myBuilding,1);
		fflush(stdout);
		delay(1000);

		//If necessary, stop the elevator to let people on/off
		if(myBuilding.elevator.currentFloor == myBuilding.elevator.nextStop)
		{
			//Redraw the building, with elevator doors open
			system("clear");
			drawBuilding(myBuilding,0);
			fflush(stdout);
			delay(1000);
			i++;

			//Let people on/off the elevator
			stopElevator(&myBuilding);

			//Redraw the building, with elevator doors open, showing the change
			system("clear");
			drawBuilding(myBuilding,0);
			fflush(stdout);
			delay(1000);
			i++;

			//Redraw the building, with elevator doors closed
			system("clear");
			drawBuilding(myBuilding,1);
			fflush(stdout);
			delay(1000);
			i++;
		}

		//Check if all people have been taken to their correct floors
		int8_t numPeopleAtDestination = 0;
		for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
		{
			numPeopleAtDestination += myBuilding.floors[f].arrivals;
		}

		if(numPeopleAtDestination == 10)
		{
			printf("SUCCESS: You got all the people through! The total time was: %i seconds!\n", i);
			return;
		}
	}

	//If the animation time's out, let them know their score.
	int8_t numPeopleAtDestination = 0;
	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		numPeopleAtDestination += myBuilding.floors[f].arrivals;
	}
	printf("FAIL: You ran out of time. You got %i/10 people through in 60 seconds.\n", numPeopleAtDestination);
}

//****************************************************************************
// Private function(s):
//****************************************************************************
static void initBuilding(void)
{
	memset(&myBuilding, -1, sizeof(struct building_s));

	myBuilding.elevator.currentFloor = rand() % BUILDING_HEIGHT;
	myBuilding.elevator.nextStop = myBuilding.elevator.currentFloor;

	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		for(int8_t j = 0; j < 2; j++)
		{
			int destination = rand() % BUILDING_HEIGHT;
			while(destination == f)
			{
				destination = rand() % BUILDING_HEIGHT;
			}
			myBuilding.floors[f].departures[j] = destination;
		}
		myBuilding.floors[f].arrivals = 0;
	}
}

static void moveElevator(struct elevator_s * elevator)
{
	if(elevator->nextStop < elevator->currentFloor)
	{
		elevator->currentFloor--;
	}
	else if(elevator->nextStop > elevator->currentFloor)
	{
		elevator->currentFloor++;
	}
}

static void stopElevator(struct building_s * building)
{
	//Let out anyone who needs to get off
	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(building->elevator.passengers[i] == building->elevator.currentFloor) 
		{
			building->floors[building->elevator.currentFloor].arrivals++;
			building->elevator.passengers[i] = -1;
		}
	}

	//Let in anyone who needs to get on, but only if capacity allows
	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(building->elevator.passengers[i] == -1) 
		{
			for(int8_t j = 0; j < 2; j++)
			{
				if(building->floors[building->elevator.currentFloor].departures[j] != -1)
				{
					building->elevator.passengers[i] = building->floors[building->elevator.currentFloor].departures[j];
					building->floors[building->elevator.currentFloor].departures[j] = -1;
					break;
				}
			}
		}
	}
}

static void drawBuilding(struct building_s building, int8_t doorStatus)
{
	printf("          DEPARTURES|     |ARRIVALS\n");

	for(int8_t f = 0; f < BUILDING_HEIGHT; f++)
	{
		drawFloor(building.floors[f],f,building.elevator,doorStatus);
	}
}

static void drawFloor(struct floor_s floor, int8_t floorNumber, struct elevator_s elevator, int8_t doorStatus)
{

	printf("FLOOR %i:          ", floorNumber);
	
	for(int8_t i = 0; i < 2; i++)
	{
		if(floor.departures[i] == -1){ printf(" "); }
		else{ printf("%i",floor.departures[i]); }
	}

	if(elevator.currentFloor == floorNumber){ drawElevator(elevator, doorStatus); }
	else{printf("|     |"); }

	for(int8_t i = 0; i < floor.arrivals; i++)
	{
		printf("x");
	}

	printf("\n");
}

static void drawElevator(struct elevator_s elevator, int8_t doorStatus)
{
	if(doorStatus == 1){ printf("| "); }
	else{ printf("  "); }

	for(int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if(elevator.passengers[i] == -1){ printf("_"); }
		else{ printf("%i",elevator.passengers[i]); }
	}

	if(doorStatus == 1){ printf(" |"); }
	else{ printf("  "); }
}

static void delay(int16_t ms)
{
    //Storing start time
    clock_t startTime = clock();
 
    //Looping till required time is not achieved
    while(clock() < startTime + ms);
}
