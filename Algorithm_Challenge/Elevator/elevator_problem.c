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

static int number_departures(struct building_s building,int floor){
	int nDepartures = 0;
	for(int i=0;i<FLOOR_MAX_DEPARTURES;i++){ 
		if (building.floors[floor].departures[i]!=-1){
			nDepartures++;
		}
	}
	return nDepartures;
}

static int number_passengers(struct elevator_s elevator){
	int nPassengers = 0;
	for(int i=0;i<ELEVATOR_MAX_CAPACITY;i++){
		if (elevator.passengers[i]!=-1){
			nPassengers++;
		}
	}
	return nPassengers;
}

//Returns the floor the elevator should STOP at next
//To stop at a floor means to open the doors and let passengers on and off. It 
//is possible to pass through a floor without stopping there.
//Note: The output should be a number between 0 and (BUILDING_HEIGHT-1), inclusive
static int8_t setNextElevatorStop(struct building_s building)
{
	// return rand()%5; // Test changing elevator. Works (but too slow as expected).

	//If not at the target floor yet, don't redecide: keep going. Avoid second guessing/possible logic jitter.
	if (building.elevator.nextStop != building.elevator.currentFloor){
		return building.elevator.nextStop;
	}

	int8_t nDepartures[BUILDING_HEIGHT];
	for (int i=0;i<BUILDING_HEIGHT;i++){
		nDepartures[i]=number_departures(building,i);
	}

	//If at current floor and have no people on the elevator, and there are departures here, pick them up.
	if (building.elevator.nextStop == building.elevator.currentFloor){
		if (number_passengers(building.elevator)==0){
			if (nDepartures[building.elevator.currentFloor]>0){
				return building.elevator.currentFloor;
			}
			else{ // find which floor (best?) to move to with people waiting.
				//Farthest floors have highest cost to transport those people. Move there first so not bouncing to farthest floors at end?
				//Nearest floors have lowest travel costs.
				// if at 0 or 4, check in sequence.
				// if at 1 or 3, check 0/4 then 2/2-> 4/0
				// if at 2, check for most passengers at 0/4, then 1/3
				switch(building.elevator.currentFloor){
					case 0:
						for (int i=1;i<BUILDING_HEIGHT;i++){
							if (nDepartures[i]>0){
								return i;
							}
						}
						break;
					case 1:
						if (nDepartures[0]>0){
							return 0;
						}
						for (int i=2;i<=4;i++){
							if (nDepartures[i]>0){
								return i;
							}
						}
						break;
					case 2:
						if (nDepartures[0]==2){
							return 0;
						}
						if (nDepartures[4]==2){
							return 4;
						}
						if (nDepartures[0]==1){
							return 0;
						}
						if (nDepartures[4]==1){
							return 4;
						}
						if (nDepartures[1]==2){
							return 1;
						}
						if (nDepartures[3]==2){
							return 3;
						}
						if (nDepartures[1]==1){
							return 1;
						}
						if (nDepartures[3]==1){
							return 3;
						}
						//No one left!?
						return 2;
						break;
					case 3:
						if (nDepartures[4]>0){
							return 4;
						}
						for (int i=2;i>=0;i--){
							if (nDepartures[i]>0){
								return i;
							}
						}
						break;
					case 4:
						for (int i=3;i>=0;i--){
							if (nDepartures[i]>0){
								return i;
							}
						}
						break;
				}
			}
		}
		else{ //Go to floor of one of the passengers.
			//Go to nearest destination.
			//If multiple nearest destinations? take into account number of departures at destination and current elevator capacity.
			int targetFloor = -1;
			for (int i=0;i<ELEVATOR_MAX_CAPACITY;i++){
				if (building.elevator.passengers[i]!=-1){
					if(targetFloor==-1){
						targetFloor = building.elevator.passengers[i];
					}
					else{
						if (abs(targetFloor-building.elevator.currentFloor)>abs(building.elevator.passengers[i]-building.elevator.currentFloor)){
							targetFloor = building.elevator.passengers[i];
						}
					}
				}
			}
			return targetFloor;
		}
	}
	return rand()%5; //Shouldn't get here anymore.
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
