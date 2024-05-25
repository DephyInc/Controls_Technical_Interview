# Traffic Light Problem (1 hour)

There's a busy traffic intersection outside your building. The roads at the intersection are horizantal street and vertical street. 
You are an engineer with the department of transportation and you're trying to come up with a new traffic light pattern that 
prevents cars from crashing in the intersection but also minimizes traffic jams. You ask a junior engineer to come up with a new 
traffic pattern and simulate the traffic flow at the intersection for two minutes or until 40 cars pass through. They work on it 
but then come to you saying they're stuck and they can't get the simulation to work. The traffic lights are exhibiting erratic 
behavior, crashes aren't reported properly, and their output results of cumulative time waiting look off. They ask you to review 
their code and help them make it work.

Your goal:
- Find all the errors in the code to get the simulation working properly
- Give feedback on how to make the code better next time

You will additionally be assessed on:
- The quality of your code

A few things to note about where the junior engineer is 98% certain there are NOT errors:
- There should be no issues with the 'advanceLane' function or the functions that start with 'draw'

A few other things to note about the simulation:
- Cars waiting to go into the intersection can only move into the intersection if the light is green.
- Cars already in the intersection can move out of the intersection even when the light is red or yellow.
- A crash should occur if a car on horizantal street and a car on vertical street are in the intersection at the same time.
- Cars IN the intersection are denoted with an 'X'. Cars on either side of the intersection are denoted with a '+'. 
  This will make more sense once you try running the program.
- Only one car from each lane can be IN the intersection at once. For example, if two cars are waiting in the eastbound 
  lane to cross the intersection, they cannot both move into the intersection at the same time. The first car in the 
  lane will move into the intersection. Then, the second car will move into the intersection as the first car leaves. 

To build the project, type the following in your bash terminal:
gcc traffic_light_problem.c -o traffic_light_problem

To run the project, type the following in your bash terminal:
./traffic_light_problem.exe

Candidate Action Items:
- Find the errors in the code to make the simulation work as the junior engineer intended
- Make any other improvements to the code as you see fit
- Answer the following questions:
  - What would you tell the junior engineer about ways that they could improve their code in the future? (1-3 sentences)
    - Check the code initialization. and walk through the code and check the logic. ( I saw a nested if statement in the traffic code that was not assigning the correct value to the variable)
    - I observed that code had syntax errors such as missing break statements in the switch case. which is a common mistake. But a good practice is to use the linter to catch these errors.
    - Common place the code will stop running, please check the break logic. I saw the logic for the crash was not implemented correctly. 
    - Begin of the function explain the code in a comment. this will help a new person to understand the code.
  - How else would you change this code to make it better so you can build on it in the future?(1-10 sentences).
    - The variables such as time counter 10, and other should be a constant/global variable which can changed from outside the function.
    - The code east and west advance line can be a single function.
    - The time involved in counting the time for each light can be a global variable. so that designer can adjust that time from outside the function.
    - The code does not have to pause the simulation for the user to see the output. The code can be run in a loop and the output can be printed in the console.
    - There is no logging in the code. This will help the developers and testers profile the code for bottlenecks and performance isssues. 
    

Don't forget; we are interested in both your solution and your thought process.

Good Luck!