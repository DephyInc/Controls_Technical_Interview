# Elevator Problem (1 hour)

It's lunch time in your building and the elevator's broken. You are an engineer with elevator maintainance department and
you're trying fix the elevator and program it to pick up and drop off the people in the building in the most efficient way.
You ask a junior engineer to come up with a new elevator pick up algorithm and simulate the elevator for one minute or until
10 people make it to their destination floor. They work on it but then come to you saying they're stuck and they can't get
the simulation to work. The elevator is exhibiting erratic behavior; sometimes it runs for more than a minute and when the
elevator does manage to deliver everyone to the right place it doesn't seem to properly report how much time it took to do
so. The elevator often won't go to the first floor, people arriving to new floors aren't getting tracked correctly and seem
to disappear. Also sometimes the elevator disappears? Finally, the junior engineer isn't sure about whether their algorithm
to choose the next elevator stop is really working the way they intended. If there are passengers on the elevator they want
to choose the next stop according to the first people already in the elevator. For example, deliver the first person first,
the second person second, etc. If there's no one on the elevator, they want to choose the next stop according to floor number,
assuming that floor has people to pick up. For example, if the first floor has people to pick up, go there first; if the
first floor doesn't have people to pick up, then skip to the second floor. Overall this junior engineer is very confused
and they ask you to review their code and help them make it work.

Your goal:
- Find all the errors in the code to get the simulation working properly
- Give feedback on how to make the code better next time

You will additionally be assessed on:
- The quality of your code

A few things to note about where the junior engineer is 99% certain there are NOT errors:
- There should be no issues with the 'stopElevator' function or the functions that start with 'draw'

A few other things to note about the simulation:
- The elevator should start on a random floor
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
- Find the errors in the code to make the simulation work as the junior engineer intended
- Make any other improvements to the code as you see fit
- Answer the following questions:
  - What would you tell the junior engineer about ways that they could improve their code in the future? (1-3 sentences)
    - ADD YOUR ANSWER HERE
  - You notice the algorithm the junior engineer made is not efficient and that the elevator still takes a long time to
    deliver everyone to their destination? What approach would you suggest to make the elevator run more efficiently?
    (you don't have to implement it, just describe how you would do it in detail in 1-10 sentences)
    - ADD YOUR ANSWER HERE
  - How else would you change this code to make it better so you can build on it in the future?(1-10 sentences).
    - ADD YOUR ANSWER HERE

Don't forget; we are interested in both your solution and your thought process.

Good Luck!
