%% Run the file for an example of how to use the helper functions
% If you want to actually use these functions within your function
% implementation, just copy and paste the function into your file
data_table = readtable('detectWalking/dataset/rachel_walking_3mph_left.csv');
figure;
plot(data_table.ank_ang);
title('Ankle Angle');

steps = get_step_cycle_data_from_data_table(data_table);
average_step = get_average_step(steps);
figure;
plot(average_step.ank_ang);
title('Average Ankle Angle');

figure;
plot_variable_by_step(data_table,'ank_ang');
title('Ankle Angle Per Step');

ank_ang_index = variable_name_to_column_index(data_table,'ank_ang');

%%
% turns an N x M data table of a user doing some repeated movement into a
% K x J x M cell array of step data (i.e. presumably the data we've provided
% you is somewhat periodic. this function allows you to break the data down
% and see what's happening in each period)
% variables from step k of the resulting cell array can be accessed through dot indexing the
% variable name (e.g. steps{k}.step_time)
% K = number of steps
% J = number of samples that occured in this step
% M = number of variables
function steps = get_step_cycle_data_from_data_table(data_table)
    HS = getHSindices(data_table);
    num_steps = size(HS,1)-1;
    steps = cell(num_steps,1);

    for k = 1:(num_steps)
        cycle = data_table(HS(k):HS(k+1),:);
        steps{k,1} = cycle;
    end
end

% takes K x J x M cell array of step data and averages each variable along
% the K and M axes, such that the resulting I x M table is an average model
% of the input steps
% variables in the resulting table can be accessed through dot indexing the
% variable name (e.g. data_table.step_time)
function average_step = get_average_step(steps)
    
    % determine the longest step for use in the summation and store lengths
    % of steps in the j vector.
    I = 0;
    num_steps = size(steps,1);
    j = zeros(num_steps,1);
    for k = 1:num_steps
        j(k) = size(steps{k},1);
        if j(k) > I
            I = j(k);
        end
    end

    variableNames = steps{1}.Properties.VariableNames;
    num_var = size(steps{1},2);
    average_step_array = zeros(I,num_var);
    
    for m = 1:size(steps{1},2) 
        for k = 1:num_steps
            data = zeros(I,1);
            data(1:j(k)) = steps{k}{:,m};
            average_step_array(:,m) = average_step_array(:,m) + data;
        end
        average_step_array(:,m) = average_step_array(:,m)/num_steps;
    end

    average_step = array2table(average_step_array,'VariableNames',variableNames);
end

% plots a given variable by step (i.e. presumably the data we've provided
% you is somewhat periodic. this function allows you to break the data down
% and plot each individual period)
function plot_variable_by_step(data_table,variable)
    steps = get_step_cycle_data_from_data_table(data_table);

    for k = 1:(size(steps,1))
        plot(steps{k}{:,variable})
        hold on
    end
end

% returns the indices of the rows of the data table in which a heelstrike occurs
% (heelstrike is when your foot first touches the ground at the beginning
% of a step)
function HS = getHSindices(data_table)
    data = data_table.gait_state;
    offset = 0;
    HS = [];
    for i = 2:length(data)
        if (data(i)>offset && data(i-1)<=offset)
            HS = [HS; i];
        end
    end
end

% returns the column index of the given variable name in a data_array,
function variable_index = variable_name_to_column_index(data_table, variable_name)
    variable_index = find(strcmpi(data_table.Properties.VariableNames,variable_name));
end
