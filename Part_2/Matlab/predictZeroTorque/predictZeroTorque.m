%% Implement the following function
% Description:
% Predicts when the user will receive zero ankle torque during each step of their movement

% Input:
% data_table >> an N x M table of variables collected from a single user
% while doing a single movement for multiple steps (i.e. 10 walking steps, 
% 10 running steps, etc).

% Output:
% zeroTorquePredictions >> an N x 1 double, where each element in the array
% represents whether or not data_table.ank_torque will have a zero or non-zero value
% (the element value should be 0 to predict zero torque and 1000 to predict non-zero torque)

function zeroTorquePredictions = predictZeroTorque(data_table)
    % This is a placeholder so the code will run. Replace this with your
    % implementation.
    zeroTorquePredictions = data_table.ank_torque;
end