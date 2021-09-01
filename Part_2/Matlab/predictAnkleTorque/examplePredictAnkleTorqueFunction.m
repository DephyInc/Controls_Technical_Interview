%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/chenlu_walking_3mph_left.csv');
exampleAnkleTorquePredictions = examplePredictAnkleTorque(data_table);

function exampleAnkleTorquePredictions = examplePredictAnkleTorque(data_table)
    exampleAnkleTorquePredictions = data_table.ank_torque;

    figure
    plot(data_table.ank_torque, 'b')
    hold on;
    plot(exampleAnkleTorquePredictions,'r')
end
