close all
helperFunctions;
%calculate pitch from gyroz (could be stabilized with a Kalman filter, this
%is a dirty implimentation)
stupid_pitch = cumtrapz(average_step.step_time*10^-3,average_step.gyroz);
%construct a phase variable from pitch, so that we can use shank pitch to
%predict where we are in the gait cycle
[x,y,r,theta] = phase_variable(stupid_pitch(1:end-5),average_step.step_time(1:end-5)*10^-3);
%generate a function that maps from phase variable to torque
[B,X] = expfit(theta,average_step.ank_torque(1:end-5),10);
predicted = X*B;
%visually test
figure
plot(predicted)
hold on
plot(average_step.ank_torque)