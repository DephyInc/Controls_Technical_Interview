%% Test your code by running the following
close all;
clear all;
testIsUserInStance();

function testIsUserInStance()
    files = dir('dataset');
    files = files(3:end,1);

    for i = 1:size(files,1)
        filename = files(i).name;
        data_table = readtable(strcat('dataset/',filename));

        isUserInStancePredictions = isUserInStance(data_table);

        figure(i)
        plot(data_table.gait_state, 'b')
        hold on;
        plot(isUserInStancePredictions,'r')
    end
end
