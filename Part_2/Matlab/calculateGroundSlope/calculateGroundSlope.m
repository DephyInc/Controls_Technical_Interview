%% Implement the following function
% Description:
% Predicts the slope of the ground as a % grade

% Input:
% data_table >> an N x M table of variables collected from a single user
% while doing a single movement for multiple steps (i.e. 10 walking steps, 
% 10 running steps, etc).

% Output:
% groundSlopePredictions >> an N x 1 double, where each element in the array
% represents the % grade of the ground the user is currently moving over
% (i.e. the method should be a rough prediction of the value of data_table.incline(n)) 

function groundSlopePredictions = calculateGroundSlope(data_table)
    % This is a placeholder so the code will run. Replace this with your
    % implementation.
    groundSlopePredictions = data_table.incline;
end