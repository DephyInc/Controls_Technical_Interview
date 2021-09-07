close all;
clear all;

data_table = readtable('dataset/rachel_walking_3mph_left.csv');

figure;
plot(data_table.gait_state, 'b');
hold on;
imu_mag = sqrt(data_table.accelx.^2 + data_table.accely.^2)/1000;

n = 1;
sum_x = 0;
sum_x2 = 0;
m_val = zeros(length(imu_mag),1);
s_val = zeros(length(imu_mag),1);
imu_jerk = zeros(length(imu_mag),1);
ank_accel = zeros(length(data_table.ank_vel),1);
output_data = zeros(length(imu_mag),1);

first_step = true;
heel_strike = 0;
prev_heel_strike = 0;
prev_liftoff = 0;
stance_time = 0;
flight_time = 0;

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
            
            % Scanning for liftoff event
            if heel_strike && abs(ank_accel(i-1)) > 0.5*prev_ank_accel ...
                    && i-prev_heel_strike > stance_time/2
                prev_liftoff = i;
                stance_time = prev_liftoff - prev_heel_strike;
                heel_strike = 0; % Liftoff detected
            end
            
            % Scanning for foot touchdown event
            if abs(imu_jerk(i-1)) > m_val(i-1)+5*s_val(i-1) 
                
                % Remove this value from running mean/std calculation
                sum_x = sum_x - imu_jerk(i-1);
                sum_x2 = sum_x2 - imu_jerk(i-1)^2;
                n = n-1;
                m_val(i-1) = sum_x/n;
                s_val(i-1) = sqrt((sum_x2/n) - (m^2));
                
                % Affirming foot touchdown event
                if ank_accel(i-1) > 1000 && ~heel_strike && i-prev_liftoff > flight_time/2
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
% plot(data_table.ank_vel/1000);
% plot(ank_accel/1000);
plot(output_data);
plot(imu_jerk);
plot(m_val);
plot(m_val+5*s_val,'r:');
plot(m_val-5*s_val,'r:');
% plot(data_table.ank_vel);