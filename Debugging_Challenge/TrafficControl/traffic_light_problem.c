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
static int8_t th = 0;	// This variable will be used to track how long the horizontal traffic light has spent in its current color
static int8_t th_0 = 0; // This variable will be used to track how long there has been 0 cars waiting horizontally when the light is green
static int8_t tv = 0;	// This variable will be used to track how long the vertical traffic light has spent in its current color
static int8_t tv_0 = 0; // This variable will be used to track how long there has been 0 cars waiting vertically when the light is green

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************
static void initIntersection(void);
static char * setHorizantalTrafficLight(struct intersection_s intersection);
static char * setVerticalTrafficLight(struct intersection_s intersection);
static void advanceLane(char * trafficColor, struct lane_of_cars_s * lane);
static void drawIntersection(struct intersection_s intersection);
static void drawUpperVerticalRoad(char * trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars);
static void drawLowerVerticalRoad(char * trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars);
static void drawWestboundLane(char * trafficColor, struct lane_of_cars_s westboundCars);
static void drawEastboundLane(char * trafficColor, struct lane_of_cars_s eastboundCars);
static void delay(int16_t ms);
static int8_t checkForCrashes(void);

//****************************************************************************
// Public function(s):
//****************************************************************************

//Main function to run the traffic simulation
int main(void)
{
	//Seed my random number generator
	srand(time(0));

	//Initialize the intersection
	initIntersection();

	//Run traffic through the intersection for a set period of time
	for(int8_t i = 0; i < 120; i++)
	{
		//Update the traffic lights
		myIntersection.horizantalTrafficColor = setHorizantalTrafficLight(myIntersection);
		myIntersection.verticalTrafficColor = setVerticalTrafficLight(myIntersection);

		//Advance the lanes if possible
		advanceLane(myIntersection.horizantalTrafficColor, &myIntersection.westboundCars);
		advanceLane(myIntersection.horizantalTrafficColor, &myIntersection.eastboundCars);
		advanceLane(myIntersection.verticalTrafficColor, &myIntersection.northboundCars);
		advanceLane(myIntersection.verticalTrafficColor, &myIntersection.southboundCars);

		//Draw the intersection
		system("clear");
		drawIntersection(myIntersection);
		fflush(stdout);
		delay(1000);

		//Check to make sure no cars have crashed
		if(checkForCrashes() == 1)
		{
			printf("FAIL: Car crash!\n");
			return 0;
		}

		//Check if all the cars have left the lanes
		if(myIntersection.northboundCars.carsThatHaveLeft == 10 && myIntersection.southboundCars.carsThatHaveLeft == 10 && myIntersection.westboundCars.carsThatHaveLeft == 10 && myIntersection.eastboundCars.carsThatHaveLeft == 10)
		{
			int16_t totalWaitTime = myIntersection.northboundCars.timeWaiting + myIntersection.southboundCars.timeWaiting + myIntersection.westboundCars.timeWaiting + myIntersection.eastboundCars.timeWaiting;
			printf("SUCCESS: You got all the cars through in %i/120 seconds! The total wait time was: %i seconds!\n", i, totalWaitTime);
			return 0;
		}
	}

	//If the animation time's out, let them know their score.
	int8_t totalCarsThatMadeIt = myIntersection.northboundCars.carsThatHaveLeft + myIntersection.southboundCars.carsThatHaveLeft + myIntersection.westboundCars.carsThatHaveLeft + myIntersection.eastboundCars.carsThatHaveLeft;
	int16_t totalWaitTime = myIntersection.northboundCars.timeWaiting + myIntersection.southboundCars.timeWaiting + myIntersection.westboundCars.timeWaiting + myIntersection.eastboundCars.timeWaiting;
	printf("FAIL: Traffic Jam! You ran out of time. You got %i/40 cars through in 120 seconds. The total wait time was: %i seconds.\n", totalCarsThatMadeIt, totalWaitTime);
	
	return 0;
}

//****************************************************************************
// Private function(s):
//****************************************************************************

static void initIntersection(void)
{
	myIntersection.horizantalTrafficColor = "R";
	myIntersection.verticalTrafficColor = "R";

	myIntersection.eastboundCars.popularity = 3;
	myIntersection.westboundCars.popularity = 5;
	myIntersection.northboundCars.popularity = 2;
	myIntersection.southboundCars.popularity = 4;
}

static char * setHorizantalTrafficLight(struct intersection_s intersection)
{
	// Increment time at current color
	th++;
	
	char * currentColor = intersection.horizantalTrafficColor;	// Current color of traffic light
	char * newColor = currentColor;								// Initialize new color as current color
	traffic_light_colors_t currentColorEnum = -1;				// Initialize enum for color

	switch (*currentColor)
	{
		case 'R':
			currentColorEnum = RED;
			break;
		case 'G':
			currentColorEnum = GREEN;
			break;
		case 'Y':
			currentColorEnum = YELLOW;
			break;
	}

	// Count of cars waiting in each direction
	int8_t horizantalWaitCars = intersection.eastboundCars.carsWaitingAtIntersection + intersection.westboundCars.carsWaitingAtIntersection;
	int8_t verticalWaitCars = intersection.northboundCars.carsWaitingAtIntersection + intersection.southboundCars.carsWaitingAtIntersection;

	// Total waiting time for each direction
	int16_t horizantalWaitTime = intersection.eastboundCars.timeWaiting + intersection.westboundCars.timeWaiting;
	int16_t verticalWaitTime = intersection.northboundCars.timeWaiting + intersection.southboundCars.timeWaiting;

	// Total cars that have made it through in each direction
	int8_t horizantalDoneCars = intersection.eastboundCars.carsThatHaveLeft + intersection.westboundCars.carsThatHaveLeft;
	int8_t verticalDoneCars = intersection.northboundCars.carsThatHaveLeft + intersection.southboundCars.carsThatHaveLeft;

	// Determine new light color (if light changes, reset t to 0)
	switch(currentColorEnum)
	{
		/*** HORIZONTAL LIGHT IS RED ***/
		case RED:
			// Change light from RED to GREEN if:
			//		- the horizontal cars have been waiting for longer than the vertical cars
			//		- the vertical light is red (we don't want any crashes!)
			if((horizantalWaitTime >= verticalWaitTime) && (strcmp(intersection.verticalTrafficColor,"R") == 0))
			{
				newColor = "G";
				th = 0;
				th_0 = 0;
			}

			// HOWEVER

			// Keep the light RED if:
			//		- there are 0 cars waiting horizontally and nonzero cars waiting vertically
			//		- OR if the horizontal cars that have made it through the intersection has already reached 20
			if ((horizantalWaitCars == 0 && verticalWaitCars != 0) || horizantalDoneCars == 20)
			{
				newColor = "R";
			}
			// Light can proceed to GREEN if:
			//		- there are cars waiting horizontally OR vertical is done at 20
			//		- the vertical light is red
			else if ((horizantalWaitCars != 0 || verticalDoneCars == 20) && (strcmp(intersection.verticalTrafficColor,"R") == 0))
			{
				newColor = "G";
				th = 0;
				th_0 = 0;
			}
			break;

		/*** HORIZONTAL LIGHT IS GREEN ***/
		case GREEN:
			// Change light from GREEN to YELLOW if:
			//		- there are more cars waiting vertically
			//		- OR the light has been green for more than 10 seconds
			if((horizantalWaitTime < verticalWaitTime) || th > 10)
			{
				newColor = "Y";
				th = 0;
			}

			// HOWEVER

			// Keep light at GREEN if:
			//		- the cars crossing vertically are done at 20
			if (verticalDoneCars == 20)
			{
				newColor = "G";
			}
			// Light can proceed to YELLOW if:
			//		- there have been no cars waiting horizontally for at least 1 second
			//		- there are nonzero cars waiting vertically
			else if (horizantalWaitCars == 0)
			{
				th_0++;	// Increment time that there has been 0 cars waiting horizontally
				if (th_0 > 1 && verticalWaitCars != 0)
				{
					newColor = "Y";
					th = 0;
					th_0 = 0;
				}
			} else
			{
				th_0 = 0;	// Reset time that there has been 0 cars due to light change
			}
			break;

		/*** HORIZONTAL LIGHT IS YELLOW ***/
		case YELLOW:
			// Change the light from YELLOW to RED if:
			//		- the horizontal light has been yellow for at least 1 second
			if(th > 0)
			{
				newColor = "R";
				th = 0;
				th_0 = 0;
			}
			break;
		// Set default case to RED to minimize risk of crashes
		default:
			newColor = "R";
			th = 0;	
			th_0 = 0;
	}

	return newColor;
}

static char * setVerticalTrafficLight(struct intersection_s intersection)
{
	// Increment time at current color
	tv++;

	char * currentColor = intersection.verticalTrafficColor;	// Current color of traffic light
	char * newColor = currentColor;								// Initialize new color as current color
	traffic_light_colors_t currentColorEnum = -1;				// Initialize enum for color

	switch (*currentColor)
	{
		case 'R':
			currentColorEnum = RED;
			break;
		case 'G':
			currentColorEnum = GREEN;
			break;
		case 'Y':
			currentColorEnum = YELLOW;
			break;
	}

	// Count of cars waiting in each direction
	int8_t horizantalWaitCars = intersection.eastboundCars.carsWaitingAtIntersection + intersection.westboundCars.carsWaitingAtIntersection;
	int8_t verticalWaitCars = intersection.northboundCars.carsWaitingAtIntersection + intersection.southboundCars.carsWaitingAtIntersection;

	// Total waiting time for each direction
	int16_t horizantalWaitTime = intersection.eastboundCars.timeWaiting + intersection.westboundCars.timeWaiting;
	int16_t verticalWaitTime = intersection.northboundCars.timeWaiting + intersection.southboundCars.timeWaiting;

	// Total cars that have made it through in each direction
	int8_t horizantalDoneCars = intersection.eastboundCars.carsThatHaveLeft + intersection.westboundCars.carsThatHaveLeft;
	int8_t verticalDoneCars = intersection.northboundCars.carsThatHaveLeft + intersection.southboundCars.carsThatHaveLeft;
	
	// Determine new light color (if light changes, reset t to 0)
	switch(currentColorEnum)
	{
		/*** VERTICAL LIGHT IS RED ***/
		case RED:
			// Change light from RED to GREEN if:
			//		- the vertical cars have been waiting for longer than the horizontal cars
			//		- the horizontal light is red (we don't want any crashes!)
			if((verticalWaitTime > horizantalWaitTime) && (strcmp(intersection.horizantalTrafficColor,"R") == 0))
			{
				newColor = "G";
				tv = 0;
				tv_0 = 0;
			}

			// HOWEVER

			// Keep the light RED if:
			//		- there are 0 cars waiting vertically and nonzero cars waiting horizontally
			//		- OR if the vertical cars that have made it through the intersection has already reached 20
			if ((verticalWaitCars == 0 && horizantalWaitCars != 0) || verticalDoneCars == 20)
			{
				newColor = "R";
			}
			// Light can proceed to GREEN if:
			//		- there are cars waiting vertically OR horizontal is done at 20
			//		- the horizontal light is red
			else if ((verticalWaitCars != 0 || horizantalDoneCars == 20) && (strcmp(intersection.horizantalTrafficColor,"R") == 0))
			{
				newColor = "G";
				tv = 0;
				tv_0 = 0;
			}
			break;
			
		/*** VERTICAL LIGHT IS GREEN ***/
		case GREEN:
			// Change light from GREEN to YELLOW if:
			//		- there are more cars waiting horizontally
			//		- OR the light has been green for more than 10 seconds
			if((verticalWaitTime <= horizantalWaitTime) || tv > 10)
			{
				newColor = "Y";
				tv = 0;
			}

			// HOWEVER

			// Keep light at GREEN if:
			//		- the cars crossing horizontally are done at 20
			if (horizantalDoneCars == 20)
			{
				newColor = "G";
			}
			// Light can proceed to YELLOW if:
			//		- there have been no cars waiting vertically for at least 1 second
			//		- there are nonzero cars waiting horizontally
			else if (verticalWaitCars == 0)
			{
				tv_0++;	// Increment time that there has been 0 cars waiting horizontally
				if (tv_0 > 1 && horizantalWaitCars != 0)
				{
					newColor = "Y";
					tv = 0;
					tv_0 = 0;
				}
			} else
			{
				tv_0 = 0;	// Reset time that there has been 0 cars due to light change
			}
			
			break;

		/*** VERTICAL LIGHT IS YELLOW ***/
		case YELLOW:
			// Change the light from YELLOW to RED if:
			//		- the vertical light has been yellow for at least 1 second
			if(tv > 0)
			{
				newColor = "R";
				tv = 0;
				tv_0 = 0;
			}
			break;
		// Set default case to RED to minimize risk of crashes
		default:
			newColor = "R";
			tv = 0;
			tv_0 = 0;
	}

	return newColor;
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
	drawWestboundLane(intersection.horizantalTrafficColor,intersection.westboundCars);
	printf("----------          ---------- \n");
	drawEastboundLane(intersection.horizantalTrafficColor,intersection.eastboundCars);
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

static int8_t checkForCrashes(void)
{
	int8_t isHorizantalCarInIntersection = (myIntersection.westboundCars.carsInIntersection | myIntersection.eastboundCars.carsInIntersection);
	int8_t isVerticalCarInIntersection = (myIntersection.northboundCars.carsInIntersection | myIntersection.southboundCars.carsInIntersection);

	if(isHorizantalCarInIntersection && isVerticalCarInIntersection){return 1;}
	return 0;
}
