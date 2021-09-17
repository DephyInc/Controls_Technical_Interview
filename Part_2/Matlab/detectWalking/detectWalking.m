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
    %detectWalkingPredictions = data_table.movement;
    
    %ASSUME THAT WHEN THE ACCELEROMETER MAGNITUDE IS NEAR g, THE SUBJECT IS
    %NOT WALKING
    
    %IMPLEMENTING AS A FOR LOOP TO MAKE IT CLOSER TO A REAL TIME
    %IMPLEMENTATION
    mov_avg_buffer = []; 
    latching_buffer = ones(75,1);
    N = size(data_table,1);
    detectWalkingPredictions = zeros(N,1);
    for i = 1:N
        %convert from bits to mps2 (I think it's actually g's but i'm not
        %sure)
        scaledAccelX = bits2mps2(data_table.accelx(i));
        scaledAccelY = bits2mps2(data_table.accely(i));
        scaledAccelZ = bits2mps2(data_table.accelz(i));
        %calculate magnitude acceleration
        mag_accel = norm([scaledAccelX,scaledAccelY,scaledAccelZ]);
        %populate moving average
        if(length(mov_avg_buffer) < 75)
            mov_avg_buffer = [mov_avg_buffer; mag_accel];
        else
            mov_avg_buffer = [mov_avg_buffer(2:end); mag_accel];
        end
        %calculate moving average
        mov_mean_mag_accel = mean(mov_avg_buffer);
        latching_buffer = [latching_buffer(2:end);mov_mean_mag_accel];
        if(all(latching_buffer > 1.05)) %unambiguously walking, had been greater for a while
            detectWalkingPredictions(i) = 1;
        end
        %note: could latch in other direction, i.e. keep walking until
        %we're sure we're not walking, but given that this is a robot
        %attached to a human, a false positive is more likely to trip you
        %than a false negative
    end
    
end
