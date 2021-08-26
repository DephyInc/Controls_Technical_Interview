%% Test your code by running the following
close all;
clear all;
testPredictPeakTorque();

function testPredictPeakTorque()
    files = dir('dataset');
    files = files(3:end,1);

    for i = 1:size(files,1)
        filename = files(i).name;
        data_table = readtable(strcat('dataset/',filename));
        
        peakTorquePredictions = predictPeakTorque(data_table);
        
        figure
        plot(data_table.ank_torque, 'b')
        hold on;
        plot(peakTorquePredictions,'r')
    end
end