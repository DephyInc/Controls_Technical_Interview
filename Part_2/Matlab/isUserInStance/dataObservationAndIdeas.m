% Looking at data to decide strategy
% Initial Thoughts, ankle velocity mixed with predicted torque will likely
% give us the information we need. may need to use the IMU but may not.

clear; close all; clc;
% data_table = readtable('dataset/chenlu_walking_2mph_left.csv');
data_table = readtable('dataset/rachel_running_6mph_left.csv');

%% goal
inStance = data_table.gait_state;

%% time
time = data_table.state_time - data_table.state_time(1);

%% ankle
jointAngle = data_table.ank_ang; % angle measured in counts
jointVelocity = data_table.ank_vel;
jointTorque = data_table.ank_torque;
normAnkleData = normalize([jointAngle, jointVelocity, jointTorque, inStance]);
fp(time, normAnkleData);

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
fp(jointAccel)

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
% well.

%% IMU
accel = [data_table.accelx, data_table.accely, data_table.accelz];
gyro = [data_table.gyrox, data_table.gyroy, data_table.gyroz];
fp(accel);
fp(gyro);

% potentially useful.
accely = data_table.accely;
fp(time,[accely accely.*inStance], 'Accel Y');
fp(time,normalize([accely accely.*inStance jointVelocity]), 'Accel Y and Velo');

%% Works for some speeds:
    % proto enum
SWING = 0;
EARLYSTANCE = 1;
MIDSTANCE = 2;
LATESTANCE = 3;

% vectors
time = data_table.state_time;
jointAngle = data_table.ank_ang;
jointVelocity = data_table.ank_vel;
inStance = []; % preallocation would add to speed but we are building this vector in "real time"

% number of time points
nPts = length(time);

% initial state estimation
currentState = LATESTANCE;

% Thresholds
velocityHeelStrikeThreshold = 2000;
anglePushOffThreshold = 5200;  

% "time loop"
for i = 1:nPts
    jointAngleNow = jointAngle(i);
    jointVelocityNow = jointVelocity(i);

    if currentState == SWING
        % Add to 0 inStance vector
        inStance = [inStance; 0];

        % Check if we have transistioned to early stance
        if jointVelocityNow > velocityHeelStrikeThreshold
            currentState = EARLYSTANCE;
        end
    elseif currentState == EARLYSTANCE
        % Add to 1 inStance vector
        inStance = [inStance; 1];

        % Check if we have transistioned to midstance
        if (jointVelocityNow < 0)
            currentState = MIDSTANCE;
        end
    elseif currentState == MIDSTANCE
        % Add to 1 inStance vector
        inStance = [inStance; 1];

        % Check if we have transistioned to midstance
        if (jointVelocityNow > 0)
            currentState = LATESTANCE;
        end
    elseif currentState == LATESTANCE
        % Add to 1 inStance vector
        inStance = [inStance; 1];

        % Check if we have transistioned to midstance
        if (jointVelocityNow < 0)
            currentState = SWING;
        end
    end
end

isUserInStancePredictions = inStance;


%%
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