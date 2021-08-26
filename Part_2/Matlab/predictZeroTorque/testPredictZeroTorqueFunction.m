%% Test your code by running the following
close all;
clear all;
testPredictZeroTorque();

function testPredictZeroTorque()
    files = dir('dataset');
    files = files(3:end,1);

    for i = 1:size(files,1)
        filename = files(i).name;
        data_table = readtable(strcat('dataset/',filename));
        
        zeroTorquePredictions = predictZeroTorque(data_table);
        
        figure
        plot(data_table.ank_torque, 'b')
        hold on;
        plot(zeroTorquePredictions,'r')
    end
end