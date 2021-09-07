close all;
clear all;

data_table = readtable('dataset/rachel_walking_3mph_left.csv');

figure;
plot(data_table.gait_state, 'b');
hold on;
imu_mag = sqrt(data_table.accelx.^2 + data_table.accely.^2)/1000;
% plot(imu_mag);

sum_x = 0;
sum_x2 = 0;
m_val = zeros(length(imu_mag),1);
s_val = zeros(length(imu_mag),1);
imu_jerk = zeros(length(imu_mag),1);
ank_accel = zeros(length(data_table.ank_vel),1);
output_data = zeros(length(imu_mag),1);

heel_strike = 0;
prev_ank_vel = 0;

for i = 1:length(imu_mag)
    if i > 1
        imu_jerk(i-1) = imu_mag(i) - imu_mag(i-1);
        ank_accel(i-1) = data_table.ank_vel(i) - data_table.ank_vel(i-1);
        sum_x = sum_x + imu_jerk(i-1);
        sum_x2 = sum_x2 + imu_jerk(i-1)^2;
        m = sum_x/(i-1);
        m_val(i-1) = m;
        s_val(i-1) = sqrt((sum_x2/(i-1)) - (m^2));
        
        if i > 500 % Wait 5 seconds to accumulate baseline
            if heel_strike && abs(ank_accel(i-1)) > 0.5*prev_ank_accel
                i
                heel_strike = 0; % Liftoff detected
            end
            
            if imu_jerk(i-1) > m_val(i-1)+5*s_val(i-1) && ank_accel(i-1) > 1000 && ~heel_strike
                i
                prev_ank_accel = ank_accel(i-1);
                heel_strike = 1; % Heel Strike detected
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
% plot(imu_jerk);
plot(m_val);
plot(m_val+5*s_val,'r:');
plot(m_val-5*s_val,'r:');
% plot(data_table.ank_vel);

% data = zeros(length(imu_mag),1);
% mval = 0;
% sval = zeros(length(imu_mag),1);
% m_val = zeros(length(imu_mag),1);
% for i = 1:length(imu_mag)
%     if i > 1
%         data(i) = imu_mag(i) - imu_mag(i-1);
%         mval = (mval*(i-1) + data(i))/i;
%         m_val(i) = mval;
%         sval(i) = sqrt((sval(i-1)^2*(i-1) + (data(i) - m_val(i))^2)/i);
%         
%         sval(i) =  + sqrt((data(i) - m_val(i))^2/i);
%     end
% end
% plot(data);
% plot(m_val);
% plot(m_val+10*sval,'r:');
% plot(m_val-10*sval,'r:');

% mval = 0; 
% mean_val = zeros(length(imu_mag));
% sval = zeros(length(imu_mag));
% for i = 1:length(imu_mag)
%     mval = (mval*(i-1) + imu_mag(i))/i;
%     mean_val(i) = mval;
%     sval(i) = sqrt((imu_mag(i) - mean_val(i))^2/i);
% end
% plot(1:length(mean_val),mean_val);
% plot(1:length(sval),sval);

% % figure;
% imu_jerk = diff(sqrt(data_table.accelx.^2 + data_table.accely.^2));
% ank_accel = diff(data_table.ank_vel);
% new_data = [];
% 
% for i = 1:length(ank_accel)
%     if (imu_jerk(i) > 1000 && imu_jerk(i+1)) % < -1000 && ank_accel(i) > 0)
%         new_data = [new_data i+1];
%     end
% end
% % plot(new_data, zeros(size(new_data)) + 2, 'x');
% 
% % for i = 1:length(data_table.ank_vel)
% %     if (diff(data_table.ank_vel(i)) > 0)
% %         
% %     end
% % end
% 
% % plot(ank_accel/1000);