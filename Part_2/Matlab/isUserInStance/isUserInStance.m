%% Implement the following function
% Description:
% Predicts when the user is in stance (i.e. has their foot on the ground)

% Input:
% data_table >> an N x M table of variables collected from a single user
% while doing a single movement for multiple steps (i.e. 10 walking steps,
% 10 running steps, etc).

% Output:
% isUserInStancePredictions >> an N x 1 double, where each element in the array
% represents whether or not data_table.gait_state(n) will have a value of 1
% (i.e. the method should predict a value of 1 when data_table.gait_state(n) = 1 (i.e. stance)
% and should predict a value of 0 when data_table.gait_state(n) != 1.

function isUserInStancePredictions = isUserInStance(data_table)
    % This code will run a loop through the dataset to simulate real time
    % data flowing in.
    
    % proto enum
    SWING = 0;
    EARLYSTANCE = 1;
    MIDSTANCE = 2;
    LATESTANCE = 3;
    
    % vectors
    time = data_table.state_time;
    jointAngle = data_table.ank_ang - data_table.ank_ang(1);
    jointVelocity = data_table.ank_vel;
    jointTorque = data_table.ank_torque;
    inStance = []; % preallocation would add to speed but we are building this vector in "real time"
    
    % number of time points
    nPts = length(time);
    
    % initial state estimation
    currentState = LATESTANCE;
    
    % Thresholds
    velocityHeelStrikeThreshold = 500;
    accelHeelStrikeThreshold = 80;
    velocityPushOffThreshold = 500;
    anglePushOffThreshold = 1000;      
    
    % debug
    exPred = max(0,data_table.gait_state);
    
    % "time loop"
    jointVelocityPrev = 0;
    jointVelocityNow = 0;
    filt = .9;
    for i = 1:nPts
        jointAngleNow = jointAngle(i);
        jointVelocityNow = (1-filt)*jointVelocityNow + (filt)*jointVelocity(i);
        jointAccel = (jointVelocityNow - jointVelocityPrev)/10;
        jointVelocityPrev = jointVelocityNow;
        jointTorqueNow = jointTorque(i);
        
        inStance = [inStance; (currentState ~= SWING)];
        
        % Debug
        
        if (inStance(end) ~= exPred(i))
            disp([currentState inStance(end) exPred(i) jointAngleNow jointVelocityNow]);
        end
        
        if currentState == SWING       
            % Check if we have transistioned to early stance
            if (jointVelocityNow > velocityHeelStrikeThreshold) && (jointAccel > accelHeelStrikeThreshold) %&& (jointAngleNow > 0)
                currentState = EARLYSTANCE;
            end
        elseif currentState == EARLYSTANCE
            % Check if we have transistioned to midstance
            if (jointVelocityNow < 0)
                currentState = MIDSTANCE;
            end
        elseif currentState == MIDSTANCE
            % Check if we have transistioned to midstance
            if (jointVelocityNow > velocityPushOffThreshold)
                currentState = LATESTANCE;
            end
        elseif currentState == LATESTANCE
            % Check if we have transistioned to midstance
            if (jointVelocityNow < velocityPushOffThreshold)  && (jointAngleNow > anglePushOffThreshold)
                currentState = SWING;
            end
        end
        
        % final check of Torque if prediction can be trusted.
        if (currentState == SWING && jointTorqueNow > 0)
            currentState = LATESTANCE;
        end
    end
    
    isUserInStancePredictions = inStance;
end
