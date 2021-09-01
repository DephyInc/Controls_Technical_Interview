%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/rachel_walking_3mph_left.csv');
exampleDetectWalkingPredictions = exampleDetectWalking(data_table);

function exampleDetectWalkingPredictions = exampleDetectWalking(data_table)
    exampleDetectWalkingPredictions = data_table.movement;

    figure
    plot(data_table.movement, 'b')
    hold on;
    plot(exampleDetectWalkingPredictions,'r')
end
