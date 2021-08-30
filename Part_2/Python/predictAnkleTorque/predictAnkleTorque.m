%% Implement the following function
% Description:
% Predicts how much ankle torque the user should recieve from the exo when
% walking

% Input:
% data_table >> an N x M table of variables collected from a single user
% while doing a single movement for multiple steps (i.e. 10 walking steps, 
% 10 running steps, etc).

% Output:
% ankleTorquePredictions >> an N x 1 double, where each element in the array
% represents how much ankle torque the user would like to receive at this point in the step
%(i.e. this function should predict the value or data_table.ank_torque)

function ankleTorquePredictions = predictAnkleTorque(data_table)
    % This is a placeholder so the code will run. Replace this with your
    % implementation.
    ankleTorquePredictions = data_table.ank_torque;
end