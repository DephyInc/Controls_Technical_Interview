%% Implement the following function
% Description:
% Predicts when the user is walking

% Input:
% data_table >> an N x M table of variables collected from a single user
% while doing a single movement for multiple steps (i.e. 10 walking steps, 
% 10 running steps, etc).

% Output:
% detectWalkingPredictions >> an N x 1 double, where each element in the array
% represents whether or not data_table.movement(n) will have a value of 1 (i.e. walking)
% IMPORTANT NOTE: The minimum implementation should predict a value of 1 when 
% data_table.movement(n) = 1 and should predict a value of 0 when data_table.movement(n) != 1.
% An optional, more advanced implementation would simply predict the value
% of data_table.movement(n).

function detectWalkingPredictions = detectWalking(data_table)
    % This is a placeholder so the code will run. Replace this with your
    % implementation.
    detectWalkingPredictions = data_table.movement;
end