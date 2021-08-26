%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/upstairs_fast_R.csv');
exampleZeroTorquePredictions = examplePredictZeroTorque(data_table);

function exampleZeroTorquePredictions = examplePredictZeroTorque(data_table)
    exampleZeroTorquePredictions = data_table.ank_torque;
    
    figure
    plot(data_table.ank_torque, 'b')
    hold on;
    plot(exampleZeroTorquePredictions,'r')
end