# Exo Controller - Part 1 (10 - 15 minutes)

Your team is designing an ExoBoot controller to augment someone while they’re walking.

Given the data from the sensors on the exo, which include a 6-axis IMU located on the calf, an encoder located on the ankle, a motor encoder, and a current and voltage reading from the motor, the goal is to determine what commands to send to the motor. Hopefully, those commands will result in a helpful amount of torque when a user is walking, but will not hinder the user in any way when the user is doing other motions.

1. Describe the components of the controller you design? What considerations are you making when designing/implementing the controller?

A controller built to assist a person walking would be built on top of a state machine given that there are very distinct phases in a walking gait that require different types of control. At its most basic we can divide the gait into swing and stance phases. 

During swing, the functional requirement for the controller would be to ensure the user doesn't feel the device. This could be as simple as turning off the motor or as complex as maintaining a small level of tension on the chain that connects the motor to the ankle moment arm to make sure it doesn't "flop around." In addition we can use the swing phase to predict user intent.  Given that we have an encoder and a 6-axis IMU, we can observe the position of the foot and track the trajectory of the shank throughout a single swing. This can give us information about the type of terrain the person is walking on and how fast the person is walking / running (which we can use during stance to modulate the push-off torque). This is especially important in regards to stairs or hills as climbing requires a power additive stance phase, while descending should be power dissipative. 

The stance phase can be treated as a single state or it can be further subdivided into early, mid, and late stance. Early-stance would start when the foot touched the ground and end when the foot comes to rest (for a typical walking gait this is from when the heel strikes the ground to when the toes touch down). Mid-stance starts from the end of the early stance and ends at max dorsiflexion. Late-stance starts at max dorsiflexion and ends at toe off. It is during late-stance that we will need the ExoBoot to provide a plantarflexion torque to the user to aid in their push-off. The amount of torque we want to add and when depends factors including the users walking speed, the slope of the ground, and the roughness of the terrain. The transition between each of these phases can be made by observing the velocity of the ankle.

Late stance seems to be the place where the difficulty lies. The torque would have to start smoothly so as to not be jarring, be strong enough to provide assistance, not too strong as to cause a major kick during low intensity activities, and incredibly consistent. To accomplish this we need an accurate prediction of the ankle torque and a good understanding of the assistance required for various tasks. This would require a library of data and a predictive model of the device during operation.

Some other considerations:
a) This is an exoskeleton not an independent robot of prosthesis. This means we have to cooperate with the user and any torque we apply will be in parallel to the ankle. Therefore, we need to start with safeties.  The human ankle is very powerful and so is our equipment so we need to be 100% sure that we can't injure the person by applying a high current at the wrong time. This can be accomplished by creating robust fault states and passing all command variables through strict limits.
b) There is no direct force or torque measurement. This means we would have to predict the torque at the ankle using other sensors. This could be done using an internal model of the person in conjuction with the ankle encoder and the IMU.
c) Signals from sensors are noisy. We will need to make sure that the data we receive from the sensors are filtered. For the IMU in particular, we should implement a sensor fusion (e.g. madgwick) algorithm to help improve the orientation prediction and a zero velocity update every at the end of the early stance phase to reduce drift in position predictions.

2. If you have to split the work among your team, which component could
you contribute the most to?

I feel I could contribute to the overall state machine, the safeties, and the predictive model.

3. Which parts would you give to another team member?

While I could contribute, I would probably give other team members the task of sensor noise reduction and working with the communication Protocols.

 

---------------------------

## Candidate Action Items:

None. At this point in the process you've already answered this question.
