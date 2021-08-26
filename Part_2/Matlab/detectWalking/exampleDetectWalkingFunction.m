%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/upstairs_fast_R.csv');
exampleDetectWalkingPredictions = exampleDetectWalking(data_table);

function exampleDetectWalkingPredictions = exampleDetectWalking(data_table)
    exampleDetectWalkingPredictions = data_table.movement;
    
    figure
    plot(data_table.movement, 'b')
    hold on;
    plot(exampleDetectWalkingPredictions,'r')
end