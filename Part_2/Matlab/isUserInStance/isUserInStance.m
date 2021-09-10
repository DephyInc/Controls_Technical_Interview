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
    jointAngle = data_table.ank_ang;
    jointVelocity = data_table.ank_vel;
    jointTorque = data_table.ank_torque;
    inStance = []; % preallocation would add to speed but we are building this vector in "real time"
    
    % number of time points
    nPts = length(time);
    
    % initial state estimation
    currentState = LATESTANCE;
    
    % Thresholds
    velocityHeelStrikeThreshold = 1000;
    anglePushOffThreshold = 1000;  
    
    % "time loop"
    for i = 1:nPts
        jointAngleNow = jointAngle(i);
        jointVelocityNow = jointVelocity(i);
        jointTorqueNow = jointTorque(i);
        
        if currentState == SWING
            % Add to 0 inStance vector
            inStance = [inStance; 0];
            
            % Check if we have transistioned to early stance
            if (jointVelocityNow > velocityHeelStrikeThreshold) && (jointAngleNow > 0)
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
            if (jointVelocityNow < 0)  && (jointAngleNow > anglePushOffThreshold)
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
