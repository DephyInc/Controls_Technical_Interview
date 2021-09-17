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

function [ankleTorquePredictions] = predictAnkleTorque(data_table)
    % This is a placeholder so the code will run. Replace this with your
    % implementation.
    %ankleTorquePredictions = data_table.ank_torque;
    
    %implementing as a for loop to more closely resemble real-time
    %exoskeleton code
    
    %initialize variables
    N = size(data_table,1);
    last_GS = 0; %initialize to 0 since stand-walking edge synchronized with first HS
    shank_angle = 0;
    int_shank_ang = 0;
    ankleTorquePredictions = zeros(N,1);
    phase_variable_time = zeros(N,1);
    ankle_torque_buffer = [];
    
    mean_shank_value = -1.1157e+03; %found empirically from data set
    %exponential coefficients (see script exponential_fit for details)
    B = [-110.949062808946;...
        1086.46705936376;...
        -2397.27421093161;...
        2525.57773022321;...
        -994.542903692500;...
        198.798554202593;...
        -22.7086293284191;...
        1.54456508904014;...
        -0.0618059353425321;...
        0.00134191036380910;...
        -1.21876172370230e-05];
    phase_exp = ones(11,1);
    
    for i = 1:N
        %detect walking
        if(data_table.movement(i) ~= 0)
            %detect HS
            if((last_GS == 0) && (data_table.gait_state(i) == 1))
                int_shank_ang = 0;
                shank_ang = 0; 
            end
            last_GS = data_table.gait_state(i);
            %construct phase variable
            if(i > N)
                delta_t = 10^-3*(data_table.state_time(i+1) - data_table.state_time(i));
            else
                delta_t = 10^-3*(data_table.state_time(i) - data_table.state_time(i-1));
            end
            shank_ang = shank_ang + delta_t*data_table.gyroz(i);
            int_shank_ang = int_shank_ang + delta_t*(shank_ang-mean_shank_value);
            phase = atan2(int_shank_ang,shank_ang-mean_shank_value);
            phase_variable_time(i) = phase;
            %torque calculated from phase variable using exponential
            %regression fit (see exponential_fit for details)
            for j = 2:11
                phase_exp(j) = exp((j-1)*phase);
            end
            torque = B'*phase_exp;
           
            ankleTorquePredictions(i) = torque;
            
        end
    end
    
end
