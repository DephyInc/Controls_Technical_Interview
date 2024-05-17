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
    - Check types and max size of values expected.
	- Try to use functions and better re-use code: Reduces the amount of code that needs to be fixed/debugged and makes things (usually) easier to read.
	- Use unit tests to directly test pieces of code in smaller more managable pieces and to catch accidental code-breaks in the future. Especially for safety-critical features such as the Crash detection that you want to never hit, but must know it works even if never used.
  - How else would you change this code to make it better so you can build on it in the future?(1-10 sentences).
    - There are many hard coded values. Try to generalize them.
	- Try to find a simpler structure: Too many redundant if/else and switch cases seems unnecessary and fragile. Is the character necessary or can the enum be used more directly? Is the switch statement necessary or can a more vectorized logic be used?
	- Again, unit tests and an appropriate unit test framework is strongly recommended.
	- There are commonalities between east/west and north/south: These might be combined into a different type of structure to make the calcualtions the same for both directions. 


	MISC NOTES:
		Symptoms: 
			All lights are red frequently. Rarely is it green. 
			confirmation: cumulative time waiting does look off. Negative?
			Don't see any crash reports... (is that because there were no crashes, or because they weren't detected?)
		Simulation accuracy/viability: 
			Is this only straight? No turn lights?
			Only max of 40 cars each lane? Is this scalable? General? Realistic? If not realistic, then does it still help us evaluate the effectiveness of the solution in spite of that? (it might but we must consider it)
		
		
		Fixed "Copy and Paste" error with crash detection.
		InitIntersection: doesn't seem to initialize all the other values (cars* and timeWaiting) -> Looks like it is initialized to zeros if not specified.
		Type of timeWaiting could easily have too small of range. (max 40 cars per lane, * 120 s = 4800 s . 4800s X 4 lanes > 128 or 256 unsigned, but fits in uint16 (max 65535)). If only positive values expected, use an unsigned type to avoid unexpected sign issues (though still need to be careful of overflow). int8 seems big enough for other values esp if only 40 cars per lane allowed.
			Expecting max of 19200 wait time, but found values reported in the high 64000 range-> Looks like roll over with too small of type: advanceLane adds the carsWaitingAtIntersection with a smaller value: Without explicit typing, the value is cut off before saving it back: Changing carsWaitingAtIntersection fixes it without changing advanceLane.
			
		Total # cars leaving: 40*4 = 160 > 128 signed int: consider unsigned int 8. 
		
		Have unit tests. Test each component individually if possible: How did you know crashes weren't being detected unless you forced a crash? A dedicated test can save time so you are not waiting on "random" events that take a long time to check and may or may not prove a feature works (at least nominally).
		
		There is a lot of redundant/copied code: Consider using a function to abstract/refactor the common functionality: A diff between the setHorizantalTrafficLight and setVerticalTrafficLight showed minimal differences and errors in each: 
			Missing break statements in one.
			Odd and inconsistent if/else structure for setting enum type. Is the enum/character identification necessary? Use the enum everywhere, or use the character everywhere. 
		Multiple exit criteria with similar calculations meant needing to fix a bug in multiple places. The wait time calculation could be a function or have a different exit criteria that allows the calculation to happen in one spot (e.g. a success indicator with a break statement in the for).
		
		Can simulate faster than real time... Have a switch to change between realtime sim and faster than realtime sim. (Check that other code not reliant on actual timing, only computer "ticks", which it looks like is true in the advanceLane function (i.e. not reliant on computer/wall time, only ticks/cycles).
		


Don't forget; we are interested in both your solution and your thought process.

Good Luck!