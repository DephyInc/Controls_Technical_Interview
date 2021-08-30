%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/rachel_walking_3mph_left.csv');
exampleIsUserInStancePredictions = exampleIsUserInStance(data_table);

function exampleIsUserInStancePredictions = exampleIsUserInStance(data_table)
    exampleIsUserInStancePredictions = max(0,data_table.gait_state);
    
    figure
    plot(data_table.gait_state, 'b')
    hold on;
    plot(exampleIsUserInStancePredictions,'r')
end