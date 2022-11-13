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

***

### **Solution - Shannon Moffat**

I identified and fixed the following to fix the simulation function:

1. Fixed the syntax in the `traffic_light_colors_t` enum declaration
2. Fixed the nesting of if/else statements in the `setHorizantalTrafficLight` and `setVerticalTrafficLight` functions
3. Added missing breaks in the `currentColorEnum` case statement in the `setHorizantalTrafficLight` function
4. Moved the `t` variable intialization outside of functions to stop resetting to zero at every loop (then split them into unique time variables for the horizontal `th` and vertical `tv` lights)
5. Changed data type of `timeWaiting` to `int16_t` to increase range beyond -127 to 128

I did edit the `main` function, just to change the return type to `int` as noted when compiling in GCC. I saved a text file with that output in the */solution_outputs* folder for your reference.

***

- Make any other improvements to the code as you see fit
- Answer the following questions:
  - What would you tell the junior engineer about ways that they could improve their code in the future? (1-3 sentences)

***

### **Solution - Shannon Moffat** (cont.)

I would first let the junior engineer know that, overall, the code was sound and well-written other than the bugs that were identified. I appreciated the intuitively-named variables and functions, as well as the existing comments in some of the functions.

To improve their code in the future, I would recommend more incremental changes and tests to ensure that the basic elements of the code function as expected before making things more complex. Some issues like the `traffic_light_colors_t` enum declaration and the case break statements were likely overlooked in favor of building out the rest of the code.

I would also recommend giving more attention to data types, not only to prevent issues like the limitation of using `int8_t` for `timeWaiting`, but also to simplify the code overall. For example, the `traffic_light_colors_t` enum seemed redundant given the `char*` type variables that stored the colors of the traffic lights, and it could likely be eliminated from the code with a minimal reworking.

There are also some repetitive parts of the code, particularly between the two functions `setHorizantalTrafficLight` and `setVerticalTrafficLight`. I would recommend reworking this into a single function that perhaps takes the input of 'horizontal' or 'vertical'. This would reduce not only lines of code, but also the risk of copy/paste errors when editing the function body.

***

  - How else would you change this code to make it better so you can build on it in the future?(1-10 sentences).

***

### **Solution - Shannon Moffat** (cont.)

I did do a little work to improve the algorithm for the Traffic Control problem, by taking not just the number of cars waiting into account for the light change, but also:

* The total amount of time cars have been waiting in each direction
* If there are 0 cars left waiting at the intersection
* If the total cars that have made it through the intersection has reached the goal (20)

This created the following new cases for the light change:

If the horizontal traffic light is RED:
1. The light can change from RED to GREEN if:
  * the horizontal cars have been waiting for longer than the vertical cars, AND
  * the vertical light is red (to prevent crashes)
2. HOWEVER, we can keep the light RED if:
  * there are 0 cars waiting horizontally and nonzero cars waiting vertically, OR
  * 20 cars have already made it through the intersection horizontally
3. The light can still proceed to GREEN if:
  * there are nonzero cars waiting horizontally OR 20 cars have already made it through the intersection vertically, AND
  * the vertical light is red

If the horizontal traffic light is GREEN:
1. The light can change from GREEN to YELLOW if:
  * the vertical cars have been waiting for longer than the vertical cars, OR
  * the light has been green for more than 10 seconds
2. HOWEVER, we can keep the light GREEN if:
  * 20 cars have already made it through the intersection vertically
3. The light can still proceed to YELLOW if:
  * there have been no cars waiting horizontally for at least 1 second, AND
  * there are nonzero cars waiting vertically

If the horizontal traffic light is YELLOW:
1. The light can change from YELLOW to RED if:
  * the horizontal light has been yellow for at least 1 second

These cases also apply to the vertical traffic light, with complementing conditions.

Below is a statistical analysis of the time to get 40 cars through the intersection (goal is < 120s) and the total waiting time after running the simulation 30 times.

|Measure|N|Statistics|Min|Max|Mean|Median|Standard Deviation|
|-|-|-|-|-|-|-|-|
|Traffic Time|30|(in seconds)|40|93|55|51|12.21|
|Cumulative Waiting Time|30|(in seconds)|120|245|178|175|28.14|

There are further improvements that can be made, not only to the algorithm efficiency but also the code structure (as noted in the section above). In the real world, we would want to make sure no 1 car is waiting for too long (here we are looking at the total cumulative waiting time), and things would get a lot more complicated if turns were allowed or if there were any energy conservation requirements for the traffic light changes.

In the folder */solution_outputs*, I have saved a few text files with the output of my algorithm solution. One shows the traffic problem achieved with 119 seconds of waiting time, and the other shows that with 188 seconds of waiting time.

***

Don't forget; we are interested in both your solution and your thought process.

Good Luck!