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
    - Improving readability: see note on using a variable for total north and southbound so it's less difficult to read
    - Add more comments throughout to explain your thinking process
    - Watch for things like missing breaks, integer overflow, what needs to get accessed inside or outside a function, and of course the annoying typos!
  - How else would you change this code to make it better so you can build on it in the future?(1-10 sentences).
    - The light timeout should be a variable that can change and needs to be thought through more (e.g. have a min light time and a max light time and spend some time tuning those variables).
    - The light itself could be a struct that stores both the light's current condition as well as the time it's been in that condition. This change will make it easier to implement a min time and a max time and get rid of the need for additional counters.
    - Most of the code between horizantal and vertical is copied between the two; using an external function that could be used by both would be helpful (e.g. a change light function could be useful in multiple places and the order is the same R->G->Y so it can just cycle through those values)

Don't forget; we are interested in both your solution and your thought process.

Good Luck!