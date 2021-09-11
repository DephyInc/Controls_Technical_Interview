% This file contains my notes and some stream of concious work to try and
% understand the data provided and how best to manipulate it.

% Initial Thoughts, ankle velocity mixed with predicted torque will likely
% give us the information we need. may need to use the IMU but may not.

clear; close all; clc;
% data_table = readtable('dataset/chenlu_walking_2mph_left.csv');
data_table = readtable('dataset/nick_walking_4mph_left.csv');
% data_table = readtable('dataset/rachel_running_6mph_left.csv');
% data_table = readtable('dataset/rachel_walking_3mph_left.csv');

%% goal
inStance = data_table.gait_state;

%% time
time = data_table.state_time - data_table.state_time(1);

%% ankle
jointAngle = data_table.ank_ang - data_table.ank_ang(1); % angle measured in counts
jointVelocity = data_table.ank_vel;
jointTorque = data_table.ank_torque;
ankleData = ([jointAngle/max(jointAngle), jointVelocity/max(jointVelocity), jointTorque/max(jointTorque), inStance]);
fp(time, ankleData);

% from this data we find that the stance phase starts at heel strike
% signified by the ankle velocity reaching above a threshold during swing.
% This means we can detect heelstrike with just the ankle encoder data. The
% A simple implementation like this would not account for the user randomly
% pointing their toe in the air or scuffing their heel on a rock. May need
% another check in a real time device to look out for these occurances.

% We also find that the end of stance can be predicted by a negative ankle
% velocity when the ankle angle is about a threshhold. This indicates the
% foot has finished the push-off at the end of late-stance. Depending on
% the speed, the angle check may not pass so it may be better to check for
% late stance prior to checking for the change in ankle velocity sign to
% preclude initial plantarflexion without requiring a higher angle
% threshold at late stance. 

% Depending on the torque calculation we use to predict the ankle torque,
% we may be able to use the torque signal instead to predict the end of
% stance as it exactly coincides to when torque goes to zero.


%% ankle acceleration
jointAccel = (jointVelocity - [0; jointVelocity(1:end-1)])/10;
fp(time, [jointAngle, jointVelocity, jointAccel*10], 'Raw joint data')

% filter
Wn = 10/(100/2);
[b,a] = butter(1, Wn,'high');
fp(time, [jointAngle/max(jointAngle), jointVelocity/max(jointVelocity), filter(b,a,jointAccel/max(jointAccel))])

% We could also use joint acceleration to predict the rapid plantarflexion
% that acompanies heelstrike. Although there is a high plantarflexion
% acceleration at the end of the push-off phase which could cause issues if
% there is no time out before the check is made.

%% determine thresholds
fp(time,[jointAngle jointAngle.*inStance], 'Angle'); % push-off threshold = 5200
fp(time,[jointVelocity jointVelocity.*inStance], 'Velocity'); % heelstrike threshold = 2000
fp(time,[jointAccel jointAccel.*inStance], 'Accel');

% these only work for specific tests. slower speeds or running can screw
% with this prediction. we need to try to detect early and mid stance as
% well. The other issue (that I am realizing way too late) is that the zero point for the ankle angle is not
% consistent between users or gaits so I can't use positive / negative
% ankle angle to determine phase of gait.

%% IMU
accel = [data_table.accelx, data_table.accely, data_table.accelz];
gyro = [data_table.gyrox, data_table.gyroy, data_table.gyroz];
fp(accel);
fp(gyro);

% potentially useful.
accely = data_table.accely;
fp(time,[accely accely.*inStance], 'Accel Y');
fp(time,normalize([accely accely.*inStance jointVelocity]), 'Accel Y and Velo');

accelSag = sqrt(data_table.accely.^2 + data_table.accelz.^2);
fp(time, [accelSag accelSag.*inStance], 'Accel Sagittal')

% filter
Wn = 20/(100/2);
[b,a] = butter(1, Wn,'low');
fp(time, [accely, filter(b,a,accely)])

%% testing implementation
isUserInStancePredictions = isUserInStance(data_table);
fp(time, [data_table.gait_state isUserInStancePredictions], 'compare to gait state');
fp(time, [jointAngle jointAngle.*isUserInStancePredictions jointAngle.*inStance], 'joint angle prediction');
fp(time, [jointVelocity jointVelocity.*isUserInStancePredictions jointVelocity.*inStance], 'joint velocity prediction');
fp(time, normalize([jointAngle, jointVelocity, max(inStance,0), isUserInStancePredictions]));
% I need to take into acount ground slope for this to work.

%% helper function for rapid plotting
function [p,x] = fp(varargin)
% creates figure and plots current variable with title.

if (nargin == 1)
    x = varargin{1};
    figure;
    p = plot(x);
    title(inputname(1));   
elseif (nargin == 2)
    t = varargin{1};
    x = varargin{2};
    figure;
    p = plot(t,x);
    title([inputname(1) ' v ' inputname(2)]);
elseif (nargin == 3)
    t = varargin{1};
    x = varargin{2};
    figure;
    p = plot(t,x);
    title(varargin{3});
end

end