%% Run the file for an example of how to use the helper functions
% If you want to actually use these functions within your function
% implementation, just copy and paste the function into your file

clear all
close all

data_table = loadCsvFile('detectWalking/dataset/rachel_walking_3mph_left.csv');
figure(1)
plot(data_table.ank_ang)
title('Ankle Angle')

steps = stepCycle(data_table);
average_step = getAverageSteps(steps);
figure(2)
plot(average_step.ank_ang);
title('Average Ankle Angle')

figure(3)
plotVarByStep(data_table,'ank_ang');
title('Ankle Angle by Step')

ank_ang_index = varColumnIndex('ank_ang');

%% Overall helperFunctions.m Changes
% Added additional comments
% Made functions names concise and have a consistent format (functions no
% longer use underscores)


%%
% Function name: loadCsvFile
% Input: file_path
% Output: data_table
% Desciption: loads a csv file of data into an N x M table array
% N = number of samples, rows
% M = number of variables, columns
% variables in the resulting table can be accessed through dot indexing the
% variable name (e.g. data_table.step_time)

% Changes: added additional comments
function data_table = loadCsvFile(file_path)
    data_table = readtable(file_path);
end

%---------------------------------------------------------------------------------------
% Function name: stepCycle
% Input: data_table
% Output: steps
% Desciption: turns an N x M data table of a user doing some repeated movement into a
% K x J x M cell array of step data (i.e. presumably the data we've provided
% you is somewhat periodic. this function allows you to break the data down
% and see what's happening in each period)
% variables from step k of the resulting cell array can be accessed through dot indexing the
% variable name (e.g. steps{k}.step_time)
% K = number of steps
% J = number of samples that occured in this step
% M = number of variables

% Changes: renamed function to be more concise, added additional comments
function steps = stepCycle(data_table)
    HS = getHSindices(data_table);

    steps = cell((size(HS,1)-1),1); % cell array, rows = number of heelstrikes - 1, 1 column

    for k = 1:(size(HS,1)-1)
        cycle = data_table(HS(k):HS(k+1),:); % the current cycle from current index of HS to next index of HS
        steps{k,1} = cycle; % put the current cycle in the kth cell
    end
end

%---------------------------------------------------------------------------------------
% Function name: getAverageSteps
% Input: steps
% Output: average_step
% Desciption: takes K x J x M cell array of step data and averages each variable along
% the K and M axes, such that the resulting I x M table is an average model
% of the input steps
% variables in the resulting table can be accessed through dot indexing the
% variable name (e.g. data_table.step_time)

% Changes: added additional comments, renamed I to largest_step_val
function average_step = getAverageSteps(steps)
    largest_step_val = 0; % holds the size of the largest matrix in steps
    % Find the matrix with the most rows in steps
    for k = 1:(size(steps,1)) % iterate from 1 to K
        j = size(steps{k},1); % get size of the matrix at the current row of steps
        if j > largest_step_val % save the larger value in largest_step_val
            largest_step_val = j; 
        end
    end

    variableNames = steps{1}.Properties.VariableNames; 
    average_step = zeros(largest_step_val,size(steps{1},2)); % create a matrix for average step data

    for m = 1:size(steps{1},2) % iterate from 1 to the number of columns
        data = zeros(largest_step_val,1); 
        for k = 1:(size(steps,1)) % iterate from 1 to the number of the matrices in steps
            for j = 1:size(steps{k},1) % iterate from 1 to the size of the kth matrix in steps
                data(j,1) = data(j,1) + steps{k}{j,m}; % accumulate data
            end
        end
        data = data/(size(steps,1)); % average data
        average_step(:,m) = data; % save in average data
    end

    average_step = array2table(average_step,'VariableNames',variableNames);
end

%---------------------------------------------------------------------------------------
% Function: plotVarByStep
% Input: data_table, variable
% Output: n/a
% Description: plots a given variable by step (i.e. presumably the data we've provided
% you is somewhat periodic. this function allows you to break the data down
% and plot each individual period)

% Changes: added additional comments
function plotVarByStep(data_table,variable)
    HS = getHSindices(data_table); % indices of heel strike
    steps = cell((size(HS,1)-1),1); % create a cell array of matrices, # rows = # heel strikes - 1 
    %(since there is one less section between heelstrikes than there are heelstrikes, 1 column

    for k = 1:(size(HS,1)-1) % from 1 to the HS size - 1
        cycle = data_table(HS(k):HS(k+1),:); % all columns
        steps{k,1} = cycle;
    end

    for k = 1:(size(steps,1))
        plot(steps{k}{:,variable})
        hold on
    end
end
%---------------------------------------------------------------------------------------
% Function: getHSindices
% Input: data_table
% Output: HS
% Description: returns the indices of the rows of the data table in which a heelstrike occurs
% (heelstrike is when your foot first touches the ground at the beginning
% of a step, gait_state will typically go from 0 to 1)

% Changes: added additional comments, changed data(i-1)<=offset to
% data(i-1)==offset so that a change from a value <0 to 1 is not included in
% the ZC array (in case there is a change from an unknown value to 1),
% changed for i = 2:length(data) to for i = 1:length(data) so
% that the first row would be included
function HS = getHSindices(data_table)
    HS = DetectPosZC(0,data_table.gait_state); % detects when gait_state changes from a value larger than 0 (1 = stancetime) to a value = 0

    % Function: DetectPosZC (Detect position of zero crossing)
    % Input: offset, data
    % Output: ZC
    % Description: returns an array that contains the indices of the data
    % where a change occurs, based on the offset value inputted
    function ZC = DetectPosZC(offset,data)
        ZC = []; % creates an array
        for i = 1:length(data) % from row 1 to the last row
            if (data(i)>offset && data(i-1)==offset) % if data at position i is greater than the offset AND the data at the previous position is = offset
                ZC = vertcat(ZC,i); % vertical concatenation of ZC and i, putting i in the ZC array
            end
        end
    end
end

%---------------------------------------------------------------------------------------
% Function: varColumnIndex
% Input: variable_name
% Output: variable_index
% Description: returns the column index of the given variable name in a data_array,
% assuming the data array is in the same general format/order as the data in the csv logs

% Changes: renamed function to be more concise, added additional comments
function variable_index = varColumnIndex(variable_name)
	possible_variables = {'state_time','accelx','accely','accelz','gyrox','gyroy','gyroz','mot_ang','mot_vel','mot_acc','mot_cur','mot_volt','batt_volt','batt_curr','temperature','status_mn','status_ex','status_re','ank_ang','ank_vel','ank_torque','peak_ank_torque','step_energy','step_count','step_time','gait_state','movement','speed','incline','sys_time','event flags'};

	variable_index = -1; % -1 indicates the index has been found yet

    for i = 1:length(possible_variables) % step through the possible variable names
        if strcmp(possible_variables{i},variable_name) == 1 % Find the variable name
			variable_index = i;
        end
    end
end
