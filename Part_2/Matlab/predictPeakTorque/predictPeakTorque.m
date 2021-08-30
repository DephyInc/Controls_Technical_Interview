%% Implement the following function
% Description:
% Predicts when the user will receive peak ankle torque during each step of their movement

% Input:
% data_table >> an N x M table of variables collected from a single user
% while doing a single movement for multiple steps (i.e. 10 walking steps, 
% 10 running steps, etc).

% Output:
% peakTorquePredictions >> an N x 1 double, where each element in the array
% represents whether or not data_table.ank_torque will be at it's peak value 
% for the current step and what the value of that peak will be. (i.e. peakTorquePredictions(n) = 0 
% if the torque is not supposed to peak at this point in the step and peakTorquePredictions(n) = X 
% if the torque is supposed to the have a local max of X at this point in the step. Please run the 
% example script for a visual example.)

function peakTorquePredictions = predictPeakTorque(data_table)
    % This is a placeholder so the code will run. Replace this with your
    % implementation.
    peakTorquePredictions = data_table.ank_torque;
end