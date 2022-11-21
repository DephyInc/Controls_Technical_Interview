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

// Variable to hold direction of movement of elevator, initialize to 'NONE'.
static elevator_direction_t elevatorDir = NONE;

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

static elevator_direction_t determineInitialDirection(struct building_s * building);
static boolean_t isElevatorEmpty(struct building_s * building);
static int8_t findClosestFloorWithDepartures(struct building_s * building);
static boolean_t checkFloorForDepartures(struct building_s * building, int8_t floor);
static boolean_t continueInCurDirection(struct building_s * building, elevator_direction_t curElevatorDir);
static elevator_direction_t swapElevatorDirection(elevator_direction_t curElevatorDir);
static int8_t findClosestRequestedHigherFloor(struct building_s * building);
static int8_t findClosestRequestedLowerFloor(struct building_s * building);

//****************************************************************************
// Functions You (the Interviewee) Should Edit:
//****************************************************************************

/**
 * @brief Returns the floor the elevator should STOP at next. To stop at a floor means 
 * 		  to open the doors and let passengers on and off. It is possible to pass through
 * 		  a floor without stopping there.
 * 		  
 * 		  Note: The output should be a number between 0 and (BUILDING_HEIGHT-1), inclusive
 * 
 * @param building struct holding the building and elevator information 
 * @return int8_t a number between 0 and (BUILDING_HEIGHT-1), inclusive, indicating the next
 * 		   elevator stop
 */
static int8_t setNextElevatorStop(struct building_s building)
{
	// If we are just starting the simulation, determining the first direction to move in
	if (elevatorDir == NONE)
	{
		elevatorDir = determineInitialDirection(&building);
	}

	// Else if we are not just starting the simulation and the elevator is empty, send the
	// elevator to the closest floor with passengers waiting to be picked up
	else if(isElevatorEmpty(&building) == TRUE)
	{
		return findClosestFloorWithDepartures(&building);
	}

	// Else, determine if the elevator should continue in the same direction or swap directions
	else
	{
		elevatorDir = (continueInCurDirection(&building, elevatorDir) == TRUE)
			? elevatorDir : swapElevatorDirection(elevatorDir);
	}

	// Return the closest requested floor based on the elevator direction. If direction is NONE,
	// return current floor.
	if (elevatorDir == UP)
		return findClosestRequestedHigherFloor(&building);
	else if (elevatorDir == DOWN)
		return findClosestRequestedLowerFloor(&building);
	else
		return building.elevator.currentFloor;
}

/**
 * @brief Function used for determining the initial direction of the elevator
 * 
 * @param building pointer to building object
 * @return int8_t floor number for initial floor request
 */
elevator_direction_t determineInitialDirection(struct building_s* building)
{
	// Iterate until we find a passenger
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if (building->elevator.passengers[i] != -1)
		{
			// We are taking the first passenger we see in the 'passengers' array as the
			// first priority for determining direction
			if(building->elevator.passengers[i] > building->elevator.currentFloor)
				return UP;
			else
				return DOWN;
		}
	}

	// If no passengers found, we have not yet loaded anyone onboard, return NONE as default
	return NONE;
}

/**
 * @brief Function for determining if the elevator is empty
 * 
 * @param building pointer to building object
 * @return boolean_t TRUE if no passengers onboard
 */
boolean_t isElevatorEmpty(struct building_s * building)
{
	// Iterate until we find a passenger
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if (building->elevator.passengers[i] != -1)
			return FALSE;
	}

	// No passengers found, elevator is empty
	return TRUE;
}

/**
 * @brief Function for finding the closest floor with departures waiting to be picked up
 * 
 * @param building pointer to building object
 * @return int8_t closest floor with departures remaining
 */
int8_t findClosestFloorWithDepartures(struct building_s * building)
{
	// Variables to hold the current closest above and below floor to check for departures
	int8_t aboveFloor = building->elevator.currentFloor + 1;
	int8_t belowFloor = building->elevator.currentFloor - 1;

	boolean_t allFloorsExplored = FALSE;

	// Continue until we have explored all floors
	while(!allFloorsExplored)
	{
		// If the current closest above floor is valid and there are departures waiting to
		// be picked up, set as commanded floor for elevator
		if ((aboveFloor < BUILDING_HEIGHT) && checkFloorForDepartures(building, aboveFloor))
		{
			elevatorDir = UP;
			return aboveFloor;
		}

		// Else if the current closest below floor is valid and there are departures waiting to
		// be picked up, set as commanded floor for elevator
		else if ((belowFloor >= 0) && checkFloorForDepartures(building, belowFloor))
		{
			elevatorDir = DOWN;
			return belowFloor;
		}
		
		// Increment floor counters and determine if we've explored all floors
		if(++aboveFloor >= BUILDING_HEIGHT && --belowFloor < 0)
		{
			allFloorsExplored = TRUE;
		}
	}

	// No departures remaining, do not move elevator
	return building->elevator.currentFloor;
}

/**
 * @brief Function for checking if a desired floor has any departures waiting to be picked up
 * 
 * @param building pointer to building object
 * @param floor desired floor
 * @return boolean_t TRUE if the desired floor has departures waiting to be picked up
 */
boolean_t checkFloorForDepartures(struct building_s * building, int8_t floor)
{
	// Extract the desired floor
	struct floor_s desiredFloor = building->floors[floor];

	// Check to see if any departures are remaining on the desired floor
	for(int8_t j = 0; j < 2; j++)
	{
		if(desiredFloor.departures[j] != -1)
			return TRUE;
	}

	return FALSE;
}

/**
 * @brief Function for determining if the elevator should continue moving in its current
 * 		  direction. It accomplishes this by checking to see if there are any passengers
 * 		  in the elevator whose desired direction is the same as the current elevator direction.
 * 
 * @param building pointer to building object
 * @param elevatorDir current direction of elevator
 * @return boolean_t TRUE if the elevator should continue in its current direction
 */
boolean_t continueInCurDirection(struct building_s * building, elevator_direction_t curElevatorDir)
{
	// Iterate through passengers
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		if (building->elevator.passengers[i] != -1)
		{	
			// Determine the passenger's desired direction and compare against elevator direction
			elevator_direction_t passengerDesiredDir = 
				(building->elevator.passengers[i] > building->elevator.currentFloor) ? UP : DOWN;

			if(passengerDesiredDir == curElevatorDir)
				return TRUE;
		}
	}

	return FALSE;
}

/**
 * @brief Function for swapping the direction of the elevator
 * 
 * @param curElevatorDir current direction of the elevator
 * @return elevator_direction_t swapped direction of the elevator
 */
elevator_direction_t swapElevatorDirection(elevator_direction_t curElevatorDir)
{
	if(curElevatorDir == UP)
		return DOWN;
	else
		return UP;
}

/**
 * @brief Finds the closest requested higher floor among the passengers on board. That is,
 * 		  amongst the requested floors above the elevator's current floor, find the one that
 * 		  is closest to the current floor.
 * 
 * @param building pointer to building object
 * @return int8_t requested higher floor that is closest to the elevator's current floor
 */
int8_t findClosestRequestedHigherFloor(struct building_s * building)
{
	// Variable to hold the requested higher floor that is closest to the current floor
	int8_t closestRequestedHigherFloor = BUILDING_HEIGHT - 1;

	// Iterate through passengers
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		// If the current passenger wishes to go to a higher floor, but one that is closer
		// than the previous recorded closestRequestedHigherFloor, set it as the
		// closestRequestedHigherFloor
		if (building->elevator.passengers[i] != -1 && 
				building->elevator.passengers[i] > building->elevator.currentFloor &&
				building->elevator.passengers[i] < closestRequestedHigherFloor)
		{
			closestRequestedHigherFloor = building->elevator.passengers[i];
		}
	}

	return closestRequestedHigherFloor;
}

/**
 * @brief Finds the closest requested lower floor among the passengers on board. That is,
 * 		  amongst the requested floors below the elevator's current floor, find the one that
 * 		  is closest to the current floor.
 * 
 * @param building pointer to building object
 * @return int8_t requested lower floor that is closest to the elevator's current floor
 */
int8_t findClosestRequestedLowerFloor(struct building_s * building)
{
	// Variable to hold the requested lower floor that is closest to the current floor
	int8_t closestRequestedLowerFloor = 0;

	// Iterate through passengers
	for (int8_t i = 0; i < ELEVATOR_MAX_CAPACITY; i++)
	{
		// If the current passenger wishes to go to a lower floor, but one that is closer
		// than the previous recorded closestRequestedLowerFloor, set it as the
		// closestRequestedLowerFloor
		if (building->elevator.passengers[i] != -1 && 
				building->elevator.passengers[i] < building->elevator.currentFloor &&
				building->elevator.passengers[i] > closestRequestedLowerFloor)
		{
			closestRequestedLowerFloor = building->elevator.passengers[i];
		}
	}

	return closestRequestedLowerFloor;
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
