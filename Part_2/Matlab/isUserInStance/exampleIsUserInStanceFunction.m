%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/upstairs_fast_R.csv');
exampleIsUserInStancePredictions = exampleIsUserInStance(data_table);

function exampleIsUserInStancePredictions = exampleIsUserInStance(data_table)
    exampleIsUserInStancePredictions = data_table.gait_state;
    
    figure
    plot(data_table.gait_state, 'b')
    hold on;
    plot(exampleIsUserInStancePredictions,'r')
end