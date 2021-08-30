%% Run the file for an example of how to use the helper functions
% If you want to actually use these functions within your function 
% implementation, just copy and paste the function into your file
data_table = load_csv_file('detectWalking/dataset/rachel_walking_3mph_left.csv');
figure
plot(data_table.ank_ang)

steps = get_step_cycle_data_from_data_table(data_table);
average_step = get_average_step(steps);
figure
plot(average_step.ank_ang);

figure
plot_variable_by_step(data_table,'ank_ang');

%%
% loads a csv file into an N x M table array
% variables in the resulting table can be accessed through dot indexing the 
% variable name (e.g. data_table.step_time)
% N = number of samples
% M = number of variables
function data_table = load_csv_file(file_path)
    data_table = readtable(file_path);
end

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

    steps = cell((size(HS,1)-1),1);
    
    for k = 1:(size(HS,1)-1)
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
    I = 0;
    for k = 1:(size(steps,1))
        j = size(steps{k},1);
        if j > I 
            I = j;
        end
    end
    
    variableNames = steps{1}.Properties.VariableNames;
    average_step = zeros(I,size(steps{1},2));
    
    for m = 1:size(steps{1},2)
        data = zeros(I,1); 
        for k = 1:(size(steps,1))
            for j = 1:size(steps{k},1)
                data(j,1) = data(j,1) + steps{k}{j,m};
            end
        end
        data = data/(size(steps,1));
        average_step(:,m) = data;
    end
    
    average_step = array2table(average_step,'VariableNames',variableNames);
end

% plots a given variable by step (i.e. presumably the data we've provided 
% you is somewhat periodic. this function allows you to break the data down 
% and plot each individual period)
function plot_variable_by_step(data_table,variable)
    HS = getHSindices(data_table);

    steps = cell((size(HS,1)-1),1);
    
    for k = 1:(size(HS,1)-1)
        cycle = data_table(HS(k):HS(k+1),:);
        steps{k,1} = cycle;
    end
    
    for k = 1:(size(steps,1))
        plot(steps{k}{:,variable})
        hold on
    end
end

% returns the indices of the rows of the data table in which a heelstrike occurs
% (heelstrike is when your foot first touches the ground at the beginning
% of a step)
function HS = getHSindices(data_table)
    HS = DetectPosZC(0,data_table.gait_state);
    
    function ZC = DetectPosZC(offset,data)
        ZC = [];
        for i = 2:length(data)
            if (data(i)>offset && data(i-1)<=offset)
                ZC = vertcat(ZC,i); 
            end
        end
    end
end