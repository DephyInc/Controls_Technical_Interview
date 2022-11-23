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

#ifndef INC_ELEVATOR_PROBLEM_H
#define INC_ELEVATOR_PROBLEM_H

//****************************************************************************
// Include(s)
//****************************************************************************
#include <stdint.h>

//****************************************************************************
// Definition(s)
//****************************************************************************
#define ELEVATOR_MAX_CAPACITY	3
#define BUILDING_HEIGHT			5

//****************************************************************************
// Structures(s)
//****************************************************************************
struct elevator_s
{
	int8_t currentFloor;
	int8_t nextStop;
	int8_t passengers[ELEVATOR_MAX_CAPACITY];
};

struct floor_s
{
	int8_t departures[2];
	int8_t arrivals;
};

struct building_s
{
	struct floor_s floors[BUILDING_HEIGHT];
	struct elevator_s elevator;
};

//****************************************************************************
// Public Function Prototype(s):
//***************************************************************************
void main(void); //Main function to run the elevator simulation

#endif //INC_ELEVATOR_PROBLEM_H
