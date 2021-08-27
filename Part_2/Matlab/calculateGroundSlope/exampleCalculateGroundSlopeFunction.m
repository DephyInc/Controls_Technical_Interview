%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/upstairs_fast_R.csv');
exampleGroundSlopePredictions = exampleCalculateGroundSlope(data_table);

function exampleGroundSlopePredictions = exampleCalculateGroundSlope(data_table)
    exampleGroundSlopePredictions = data_table.incline;
    
    figure
    plot(data_table.incline, 'b')
    hold on;
    plot(exampleGroundSlopePredictions,'r')
end