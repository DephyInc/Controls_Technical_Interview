%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/upstairs_fast_R.csv');
examplePeakTorquePredictions = examplePredictPeakTorque(data_table);

function examplePeakTorquePredictions = examplePredictPeakTorque(data_table)
    examplePeakTorquePredictions = data_table.ank_torque;
    
    figure
    plot(data_table.ank_torque, 'b')
    hold on;
    plot(examplePeakTorquePredictions,'r')
end