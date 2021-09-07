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
    % Setup imu magnitude (can be done in real-time too each data point in
    % the for loop but for simplicity, it is done prior to the
    % "real-time" portion.
    imu_mag = sqrt(data_table.accelx.^2 + data_table.accely.^2)/1000;

    % Initialization for running mean and standard deviation
    n = 1;
    sum_x = 0;
    sum_x2 = 0;
    m_val = zeros(length(imu_mag),1);
    s_val = zeros(length(imu_mag),1);
    
    % Initialization for storage arrays
    imu_jerk = zeros(length(imu_mag),1);
    ank_accel = zeros(length(data_table.ank_vel),1);
    output_data = zeros(length(imu_mag),1);

    % Flags and storage for stance vs flight triggering
    first_step = true;
    heel_strike = 0;
    prev_heel_strike = 0;
    prev_liftoff = 0;
    stance_time = 0;
    flight_time = 0;

    % Iteration to simulate "real-time" operation
    for i = 1:length(imu_mag)
        if i > 1
            % Calculate the jerk (derivative of acceleration) and ankle
            % acceleration values
            imu_jerk(i-1) = imu_mag(i) - imu_mag(i-1);
            ank_accel(i-1) = data_table.ank_vel(i) - data_table.ank_vel(i-1);

            % Accumulate the sum of x (imu_jerk values) and x^2
            sum_x = sum_x + imu_jerk(i-1);
            sum_x2 = sum_x2 + imu_jerk(i-1)^2;
            m = sum_x/n;
            s = sqrt((sum_x2/n) - (m^2));
            n = n+1;

            m_val(i-1) = m;
            s_val(i-1) = s;

            if i > 500 % Wait 5 seconds to accumulate baseline

                % Scanning for liftoff event by checking that the
                % heel_strike flag is active (signifying the system is in a
                % step phase), the ankle acceleration is comparable to the
                % touchdown event, and an appreciable amount of time has
                % passed (based on previous steps).
                if heel_strike && abs(ank_accel(i-1)) > 0.5*prev_ank_accel ...
                        && i-prev_heel_strike > stance_time/2
                    prev_liftoff = i;
                    stance_time = prev_liftoff - prev_heel_strike;
                    heel_strike = 0; % Liftoff detected
                end

                % Scanning for foot touchdown event based on the imu jerk
                % being outside of 5x the running standard deviation
                if abs(imu_jerk(i-1)) > m_val(i-1)+5*s_val(i-1) 

                    % Remove this value from running mean/stdev calculation
                    sum_x = sum_x - imu_jerk(i-1);
                    sum_x2 = sum_x2 - imu_jerk(i-1)^2;
                    n = n-1;
                    m_val(i-1) = sum_x/n;
                    s_val(i-1) = sqrt((sum_x2/n) - (m^2));

                    % Affirming foot touchdown event through the ankle
                    % acceleration being a large positive value, the
                    % heal_strike flag being available, and an appreciable 
                    % amount of time has passed (based on previous steps). 
                    if ank_accel(i-1) > 1000 && ~heel_strike ...
                            && i-prev_liftoff > flight_time/2
                        prev_heel_strike = i;
                        if ~first_step
                            flight_time = prev_heel_strike - prev_liftoff;
                        else
                            first_step = false;
                        end
                        prev_ank_accel = ank_accel(i-1);
                        heel_strike = 1; % Heel Strike detected
                    end
                end
            end
        end

        if heel_strike
            output_data(i) = 1;
        else
            output_data(i) = 0;
        end
    end
    
    isUserInStancePredictions = output_data;
end
