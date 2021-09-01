%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/nick_walking_0incline_left.csv');
exampleGroundSlopePredictions = exampleCalculateGroundSlope(data_table);

function exampleGroundSlopePredictions = exampleCalculateGroundSlope(data_table)
    exampleGroundSlopePredictions = zeros(size(data_table.incline));

    figure
    plot(data_table.incline, 'b')
    hold on;
    plot(exampleGroundSlopePredictions,'r')
end
