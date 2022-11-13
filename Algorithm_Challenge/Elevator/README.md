# Elevator Problem (1 hour)

It's lunch time in your building and the elevator's broken. You are an engineer with elevator maintainance department and 
you're trying fix the elevator and program it to pick up and drop off the people in the building in the most efficient way.  

Your goal:
- Get all the people in the building to their destinations in less than 1 minute.

You will additionally be assessed on:
- The quality of your code
- The time it takes to for your simulation to finish

A few things to note about the simulation:
- The elevator will start on a random floor
- The elevator has a max capacity of 3 people
- The building has 5 floors
- It takes 1 second for the elevator to move to an adjacent floor (i.e. moving from floor 1 to floor 2 takes 1 second)
- It takes 3 seconds for the elevator to stop at a floor (i.e. opening the doors, letting people on/off, and closing the 
  doors takes 3 seconds)
- If the elevator stops at a floor, everyone who can get off will, and everyone who can get on will. If not everyone who 
  wants to get on will fit, the person first in line will get preference.
- The destinations of people waiting on a floor or in the elevator are represented by an array. A value of -1 in those 
  arrays means there is no one in that spot. (Note: This should only be true for instances of the 'passengers' and 
  'departures' arrays)
- People who get off the elevator and arrive on their destination floor should be visually denoted by an 'x' on the 
  arrivals side of the simulation. Within the code, the number of people who've arrived on a floor is represented as 
  an integer variable within the floor_s structure. 

To build the project, type the following in your bash terminal:
gcc elevator_problem.c -o elevator_problem

To run the project, type the following in your bash terminal:
./elevator_problem.exe

Candidate Action Items:
- Fill in the unfinshed function at the beginning of the file: 
  - int8_t setNextElevatorStop(struct building_s building) 
  If you need to add additional variables, structures, functions, etc, that's fine. However, with the exception of the 
  above function, please do not edit the code that's already in the files, unless you are extremely certain that 
  there's a bug in the code.
- Briefly describe your implementation (1-3 sentences).

***
### **Solution - Shannon Moffat**

I built up my algorithm in steps to progressively improve the efficiency, as follows:

1. Iterate through the passengers on the elevator, and set the next elevator stop as the destination that is closest to the current floor
  * This, however did not account for whether the elevator was empty, so it would not necessarily reach all remaining departures
2. If the elevator is empty, iterate through the floors and their departures, and set the next elevator stop as the closest floor that still has departures
  * We can conclude that if there are no passengers on the elevator, and no more departures, then everyone has arrived
  * We know that we will always be able to bring every departure to their arrival, but this is not the most efficient and may not be achieved in the allotted 60 seconds
3. If there are any open spots on the elevator, check if there are any departures on a floor closer than the closest passenger's destination
  * This increases our efficiency to reliably achieve the goal in 60 seconds
  * However, there still may exist some edge cases that will go overtime due to the extra time it takes to stop/load the elevator, which we may do more frequently here since we are only checking for at least one open spot on the elevator

Below is a statistical analysis of the time to achieve the goal after running the simulation 30 times.

|N|Statistics|Min|Max|Mean|Median|Standard Deviation|
|-|-|-|-|-|-|-|
|30|(in seconds)|37|55|46|47|4.68|

This solution is reliable in getting all of the passengers to their destinations in under 60 seconds, and is reasonably efficient. There are further improvements that could be made or other methods to be explored, such as prioritizing departures that are far away from their destination, taking the direction of the elevator into account, etc. Also, in the real world we would want to make sure that no one passenger is on the elevator for too long before reaching their floor.

In the folder */solution_outputs*, I have saved a few text files with the output of my algorithm solution. One shows the elevator problem achieved in just 38 seconds, while the other shows a more "unlucky" instance that took 59 seconds to achieve.

I did edit the `main` function, just to change the return type to `int` as noted when compiling in GCC. I saved a text file with that output for your reference.
***

Don't forget; we are interested in both your solution and your thought process.

Good Luck!