%% Implement the following function
% Brief Description:
% Predicts when the user is in stance (i.e. has their foot on the ground)

% Detailed Description from README.md:
% I implemented the isUserInState function by determining when heelstrike and toe off occurred for each step, during an
% iteration through the data, to mimic real-time decisions. Heelstrike was detected by determining when an ankle angle
% spike occurred after a large gyroz spike. Toe off was detected by determining when an ankle angle spike occurred after a
% battery current spike. (Note: this implementation did not function well for the isUserInStance/dataset/rachel_running_6mph_left.csv
% dataset because there was no large battery current spike, and therefore toe off could not easily be detected)

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

% A total of four features will be detected and used to determine when heelstrike and
% toe off have occured. 2 features will be used for heelstrike and 2 for toe off.
% Heelstrike Features: gyroz spike (feature HS1), ankle angle spike (feature HS2)
% Toe off Features: battery current spike (feature TO1), ankle angle spike (feature TO2)

% A spike is determined by finding when the feature changes from positive
% slope to a negative slope or vice versa, depending on the feature.

% --- Variables ---
% Note: variables written as heelstrike and toe off features to allow for
% ease of changing the features used

UNKNOWN = -4;
STANCE = 1;
SWING = 0;
gait_state = UNKNOWN; % -4 signifies unknown, 1 signifies stance, 0 signifies swing, begin with an unknown gait_state

% Heelstrike Variables

% Feature HS1
HS1_spike = 0; % ankle angle spike has occured when = 1, has not when = 0
HS1_state = 0; %initialize as 0, indicates if ankle angle is increasing or decreasing, 0 is the starting value, 1 is increasing, -1 is decreasing
HS1_state_prev = 0; % initialize as 0
HS1_data_prev = 0; % initialize as 0
HS1_min = 1200; % the minimum gyroz value require to check if a spike has occured, estimated by inspecting data plots

% Feature HS2
HS2_spike = 0; % ankle angle spike has occured when = 1, has not when = 0
HS2_state = 0; %initialize as 0, indicates if ankle angle is increasing or decreasing, 0 is the starting value, 1 is increasing, -1 is decreasing
HS2_state_prev = 0; % initialize as 0
HS2_data_prev = 0; % initialize as 0

% Toe off Variables

% Feature TO1
TO1_spike = 0; % battery current spike has occured when = 1, has not when = 0
TO1_state = 0; % initialize as 0, indicates if battery current is increasing or decreasing, 0 is the starting value, 1 is increasing, -1 is decreasing
TO1_state_prev = 0;
TO1_data_prev = 0; % initailize as 0
TO1_min = 900; % the minimum battery current required to check if a spike has occured, estimated by inspecting data plots

% Feature TO2
TO2_spike = 0; % ankle angle spike has occured when = 1, has not when = 0
TO2_state = 0; %initialize as 0, indicates if ankle angle is increasing or decreasing, 0 is the starting value, 1 is increasing, -1 is decreasing
TO2_state_prev = 0; % initialize as 0
TO2_data_prev = 0; % initialize as 0

% --- Step Through Data ---
for i = 1:size(data_table) % iterate through data
    
    % *** In Swing Phase or at Beginning of Data (when gait_state is unknown) ***
    if((gait_state == UNKNOWN) || (gait_state == SWING))
        % *** Detect Heelstrike ***
        
        % Detect HS1
        HS1_data = data_table.gyroz(i); % get the gyroz value at the ith location
        % check for gyroz spike
        if(HS1_data > HS1_data_prev)
            HS1_state = 1; % positive slope/increasing
        else
            HS1_state = -1; % negative slope/decreasing
        end
        HS1_data_prev = HS1_data; % save current data into previous data variable
        % determine if a spike has occured
        if((HS1_state < HS1_state_prev) && (HS1_data > HS1_min))  % change from positive to negative slope AND at a value larger than HS1_min
            HS1_spike = 1; % a spike has occured
        end
        HS1_state_prev = HS1_state; % save current state into previous state variable
        
        % If HS1 has occured
        if (HS1_spike == 1)
            
            % Detect HS2
            HS2_data = data_table.ank_ang(i); % get the ankle angle at the ith location
            % check for ankle angle dip
            if(HS2_data > HS2_data_prev)
                HS2_state = 1; % positive slope/increasing
            else
                HS2_state = -1; % negative slope/decreasing
            end
            % determine if a spike has occured
            if(HS2_state > HS2_state_prev) % change from negative to positive slope
                HS2_spike = 1; % a spike has occured
            end
            HS2_data_prev = HS2_data; % save current data into previous data variable
            HS2_state_prev = HS2_state; % save current state into previous state variable
            
            % If HS2 has occured
            if(HS2_spike == 1) % when ankle dip occurs, gait_state becomes 1, stance
                
                % *** Heel Strike Detected/ Beginning of Stance Phase ***
                gait_state = STANCE; 
                HS1_spike = 0; % set back to 0
                HS2_spike = 0; % set back to 0
            end
        end
        % find toe off
        
    end
    
    % *** In Stance Phase ***
    if(gait_state == STANCE)
        
        % *** Detect Toe Off ***
        
        % Detect TO1
        TO1_data_prev = data_table.batt_curr(i-1);
        TO1_data = data_table.batt_curr(i); % get the battery current at the ith location
        % check for battery current spike
        if(TO1_data > TO1_data_prev)  % change from negative to positive slope
            TO1_state = 1; % positive slope/increasing
        else
            TO1_state = -1; % negative slope/decreasing
        end
        TO1_data_prev = TO1_data; % save current data into previous data variable
        % determine if a spike has occured
        if((TO1_state < TO1_state_prev) && (TO1_data > TO1_min))  % change from positive to negative slope AND at a value larger than TO1_min
            TO1_spike = 1; % a spike has occured
        end
        TO1_state_prev = TO1_state; % save current state into previous state variable
        
        % If TO1 has occured
        if (TO1_spike == 1)
            
            % Detect TO2
            TO2_data = data_table.ank_ang(i); % get the ankle angle at the ith location
            % check for ankle angle spike
            if(TO2_data > TO2_data_prev)
                TO2_state = 1; % positive slope/increasing
            else
                TO2_state = -1; % negative slope/decreasing
            end
            TO2_data_prev = TO2_data; % save current data into previous data variable
            % determine if a spike has occured
            if((TO2_state < TO2_state_prev))  % change from positive to negative slope
                TO2_spike = 1; % a spike has occured
            end
            TO2_state_prev = TO2_state; % save current state into previous state variable
            if(TO2_spike == 1) % when ankle spike occurs, gait_state becomes 0, swing
                
                % *** Toe Off Detected/ Beginning of Swing Phase ***
                gait_state = SWING;
                TO1_spike = 0; % set back to 0
                TO2_spike = 0; % set back to 0
            end
        end
    end
    gait_state_predictions(i) = gait_state; % store current gait_state in an array of predictions
end

isUserInStancePredictions = gait_state_predictions; % store the gait state predictions in the output of the function
end
