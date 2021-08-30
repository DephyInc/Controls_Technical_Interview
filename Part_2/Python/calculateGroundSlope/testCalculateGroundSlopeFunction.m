%% Test your code by running the following
close all;
clear all;
testCalculateGroundSlope();

function testCalculateGroundSlope()
    files = dir('dataset');
    files = files(3:end,1);

    for i = 1:size(files,1)
        filename = files(i).name;
        data_table = readtable(strcat('dataset/',filename));
        
        groundSlopePredictions = calculateGroundSlope(data_table);
        
        figure
        plot(data_table.incline, 'b')
        hold on;
        plot(groundSlopePredictions,'r')
    end
end