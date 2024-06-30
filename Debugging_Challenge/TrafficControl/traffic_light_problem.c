/****************************************************************************
	[Project] Controls Technical Interview
*****************************************************************************
	[Lead developer] Rachel Harris, rharris at dephy dot com.
	[Contributors] 
*****************************************************************************
	[This file] traffic_light_problem: Changes the traffic light colors to 
	move cars through an intersection while minimizing wait time at red lights. 
****************************************************************************
	[Created] 2022-11-04 | rharris | Initial creation
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************
#include "traffic_light_problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************************************************
// Variable(s)
//****************************************************************************
struct intersection_s myIntersection;

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************
static void init(struct intersection_s* intersection);
static traffic_light_colors_t charToEnum(char* strRep);
static char* updateLight(struct intersection_s intersection, int8_t* t, char* myLight, char* otherLightColor);
static char* setHorizontalTrafficLight(struct intersection_s intersection);
static char* setVerticalTrafficLight(struct intersection_s intersection);
static void advanceLane(char * trafficColor, struct lane_of_cars_s* lane);
static void drawIntersection(struct intersection_s intersection);
static void drawUpperVerticalRoad(char * trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars);
static void drawLowerVerticalRoad(char * trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars);
static void drawWestboundLane(char * trafficColor, struct lane_of_cars_s westboundCars);
static void drawEastboundLane(char * trafficColor, struct lane_of_cars_s eastboundCars);
static void delay(int16_t ms);
static int8_t checkForCrashes(struct intersection_s intersection);

//****************************************************************************
// Public function(s):
//****************************************************************************

//Main function to run the traffic simulation
void main(void)
{
	//Seed my random number generator
	srand(time(0));

	//Initialize the intersection
	init(&myIntersection);

	//Run traffic through the intersection for a set period of time
	for(int8_t i = 0; i < 120; i++)
	{
		//Update the traffic lights
		myIntersection.horizontalTrafficColor = setHorizontalTrafficLight(myIntersection);
		myIntersection.verticalTrafficColor = setVerticalTrafficLight(myIntersection);

		//Advance the lanes if possible
		advanceLane(myIntersection.horizontalTrafficColor, &myIntersection.westboundCars);
		advanceLane(myIntersection.horizontalTrafficColor, &myIntersection.eastboundCars);
		advanceLane(myIntersection.verticalTrafficColor, &myIntersection.northboundCars);
		advanceLane(myIntersection.verticalTrafficColor, &myIntersection.southboundCars);

		//Draw the intersection
		system("clear");
		drawIntersection(myIntersection);
		fflush(stdout);
		delay(1000);

		//Check to make sure no cars have crashed
		if(checkForCrashes(myIntersection) == 1)
		{
			printf("FAIL: Car crash!\n");
			return;
		}

		//Check if all the cars have left the lanes
		if(myIntersection.northboundCars.carsThatHaveLeft == 10 && myIntersection.southboundCars.carsThatHaveLeft == 10 && myIntersection.westboundCars.carsThatHaveLeft == 10 && myIntersection.eastboundCars.carsThatHaveLeft == 10)
		{
			int16_t totalWaitTime = myIntersection.northboundCars.timeWaiting + myIntersection.southboundCars.timeWaiting + myIntersection.westboundCars.timeWaiting + myIntersection.eastboundCars.timeWaiting;
			printf("SUCCESS: You got all the cars through! The total wait time was: %i seconds!\n", totalWaitTime);
			return;
		}
	}

	//If the animation time's out, let them know their score.
	int8_t totalCarsThatMadeIt = myIntersection.northboundCars.carsThatHaveLeft + myIntersection.southboundCars.carsThatHaveLeft + myIntersection.westboundCars.carsThatHaveLeft + myIntersection.eastboundCars.carsThatHaveLeft;
	int16_t totalWaitTime = myIntersection.northboundCars.timeWaiting + myIntersection.southboundCars.timeWaiting + myIntersection.westboundCars.timeWaiting + myIntersection.eastboundCars.timeWaiting;
	printf("FAIL: Traffic Jam! You ran out of time. You got %i/40 cars through in 120 seconds. The total wait time was: %i seconds.\n", totalCarsThatMadeIt, totalWaitTime);
}

//****************************************************************************
// Private function(s):
//****************************************************************************
static traffic_light_colors_t charToEnum(char* strRep)
{
	if(strcmp(strRep,"R") == 0)
		return RED;
	else if(strcmp(strRep,"G") == 0)
		return GREEN;
	else if(strcmp(strRep,"Y") == 0)
		return YELLOW;
	else
		return RED;
}

static void init(struct intersection_s* intersection)
{
	intersection->horizontalTrafficColor = "R";
	intersection->verticalTrafficColor = "R";

	intersection->eastboundCars.popularity = 3;
	intersection->westboundCars.popularity = 5;
	intersection->northboundCars.popularity = 2;
	intersection->southboundCars.popularity = 4;
}

static char* updateLight(struct intersection_s intersection, int8_t* t, char* myLightColor, char* otherLightColor)
{
	const int8_t horizontal_cars_waiting = intersection.eastboundCars.carsWaitingAtIntersection + 
		intersection.westboundCars.carsWaitingAtIntersection;
	const int8_t vertical_cars_waiting = intersection.northboundCars.carsWaitingAtIntersection + 
			intersection.southboundCars.carsWaitingAtIntersection;
	char* newColor = "R";
	switch(charToEnum(myLightColor))
	{
		case RED:
			if((horizontal_cars_waiting >= vertical_cars_waiting) && (strcmp(otherLightColor,"R") == 0))
			{
				newColor = "G";
				*t = 0;
			}
			break;
		case GREEN:
			if((horizontal_cars_waiting < vertical_cars_waiting) || *t > 10)
			{
				newColor = "Y";
				*t = 0;
			}
			break;
		case YELLOW:
			if(*t > 1)
			{
				newColor = "R";
				*t = 0;
			}
			break;
		default:
			newColor = "R";
			*t = 0;	
			break;
	}

	return newColor;
}


static char * setHorizontalTrafficLight(struct intersection_s intersection)
{
	static int8_t t = 0;
	t++;
	return updateLight(intersection, &t, intersection.horizontalTrafficColor, intersection.verticalTrafficColor);
}

static char * setVerticalTrafficLight(struct intersection_s intersection)
{
	static int8_t t = 0;
	t++;
	return updateLight(intersection, &t, intersection.verticalTrafficColor, intersection.horizontalTrafficColor);
}

static void advanceLane(char * trafficColor, struct lane_of_cars_s * lane)
{
	//Move any cars on the leaving side of the intersection into oblivion
	//but mark them in the total lane count
	if(lane->carsLeavingIntersection > 0)
	{
		lane->carsLeavingIntersection--;
		lane->carsThatHaveLeft++;
	}

	//Move any cars in the intersection to the leaving side of the intersection
	if(lane->carsInIntersection > 0)
	{
		lane->carsInIntersection--;
		lane->carsLeavingIntersection++;
	}

	//Move a waiting car into the intersection if possible
	if((strcmp(trafficColor, "G") == 0) && (lane->carsWaitingAtIntersection > 0))
	{
		lane->carsWaitingAtIntersection--;
		lane->carsInIntersection++;
	}

	//Add cars to the lane randomly (but not more than 10 cars can ever be added to the lane ever)
	if(lane->carsWaitingAtIntersection + lane->carsInIntersection + lane->carsLeavingIntersection + lane->carsThatHaveLeft < 10)
	{
		int addCarToLane = rand();
		if((addCarToLane % lane->popularity) == 0)
		{
			lane->carsWaitingAtIntersection++;	
		}
	}

	//Keep track of how long cars have cumulatively waited at this part of the intersection
	lane->timeWaiting += lane->carsWaitingAtIntersection;
}

static void drawIntersection(struct intersection_s intersection)
{
	drawUpperVerticalRoad(intersection.verticalTrafficColor,intersection.northboundCars,intersection.southboundCars);
	drawWestboundLane(intersection.horizontalTrafficColor,intersection.westboundCars);
	printf("----------          ---------- \n");
	drawEastboundLane(intersection.horizontalTrafficColor,intersection.eastboundCars);
	drawLowerVerticalRoad(intersection.verticalTrafficColor,intersection.northboundCars,intersection.southboundCars);
}

static void drawUpperVerticalRoad(char * trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars)
{
	char * southboundCarPlaceholder;

	//Show southbound cars waiting at the intersection and northbound cars leaving the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i >= 10 - southboundCars.carsWaitingAtIntersection)
		{
			southboundCarPlaceholder  = "+";
		}
		else{southboundCarPlaceholder  = " ";}

		if(i == 9)
		{
			printf("          |  %s ! %s |          \n",southboundCarPlaceholder,trafficColor);
		}
		else if (i == 5)
		{
			if(northboundCars.carsLeavingIntersection > 0)
			{
				printf("          |  %s ! + |          \n",southboundCarPlaceholder);
			}
			else{printf("          |  %s !   |          \n",southboundCarPlaceholder);}
		}
		else{printf("          |  %s !   |          \n",southboundCarPlaceholder);}
	}

	//Show cars in the middle of the intersection
	if(southboundCars.carsInIntersection > 0)
	{
		printf("__________   X      __________ \n");
	}
	else
	{
		printf("__________          __________ \n");
	}
}

static void drawLowerVerticalRoad(char * trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars)
{
	char * northboundCarPlaceholder;

	//Show cars in the middle of the intersection
	if(northboundCars.carsInIntersection > 0)
	{
		printf("__________       X  __________ \n");
	}
	else
	{
		printf("__________          __________ \n");
	}

	//Show northbound cars waiting at the intersection and southbound cars leaving the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i < northboundCars.carsWaitingAtIntersection)
		{
			northboundCarPlaceholder  = "+";
		}
		else{northboundCarPlaceholder  = " ";}

		if(i == 0)
		{
			printf("          |  %s ! %s |          \n",trafficColor,northboundCarPlaceholder);
		}
		else if (i == 5)
		{
			if(southboundCars.carsLeavingIntersection > 0)
			{
				printf("          |  + ! %s |          \n",northboundCarPlaceholder);
			}
			else{printf("          |    ! %s |          \n",northboundCarPlaceholder);}
		}
		else{printf("          |    ! %s |          \n",northboundCarPlaceholder);}
	}
}

static void drawWestboundLane(char * trafficColor, struct lane_of_cars_s westboundCars)
{
	//Show cars moving away from the intersection
	//Note: Because we don't really care about these cars, I will only display one of them
	//even if there is more than one car moving away from the intersection
	if(westboundCars.carsLeavingIntersection > 0)
	{
		printf("    +    %s", trafficColor);
	}
	else
	{
		printf("         %s", trafficColor);
	}

	//Show cars in the middle of the intersection
	if(westboundCars.carsInIntersection > 0)
	{
		printf("       X  ");
	}
	else
	{
		printf("          ");
	}

	//Show cars waiting at the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i < westboundCars.carsWaitingAtIntersection)
		{
			printf("+");
		}
		else{printf(" ");}
	}
	printf("\n");
}

static void drawEastboundLane(char * trafficColor, struct lane_of_cars_s eastboundCars)
{

	//Show cars waiting at the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i >= 10 - eastboundCars.carsWaitingAtIntersection)
		{
			printf("+");
		}
		else{printf(" ");}
	}

	//Show cars in the middle of the intersection
	if(eastboundCars.carsInIntersection > 0)
	{
		printf("  X       ");
	}
	else
	{
		printf("          ");
	}

	//Show cars moving away from the intersection
	//Note: Because we don't really care about these cars, I will only display one of them
	//even if there is more than one car moving away from the intersection
	if(eastboundCars.carsLeavingIntersection > 0)
	{
		printf("%s   +     \n", trafficColor);
	}
	else
	{
		printf("%s         \n", trafficColor);
	}
}

static void delay(int16_t ms)
{
    clock_t startTime = clock();
 
    while(clock() < startTime + ms);
}

static int8_t checkForCrashes(struct intersection_s intersection)
{
	int8_t isHorizontalCarInIntersection = (intersection.westboundCars.carsInIntersection | intersection.eastboundCars.carsInIntersection);
	int8_t isVerticalCarInIntersection = (intersection.southboundCars.carsInIntersection | intersection.northboundCars.carsInIntersection);
	return isHorizontalCarInIntersection && isVerticalCarInIntersection;
}
