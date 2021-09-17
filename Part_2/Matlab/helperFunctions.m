%% Run the file for an example of how to use the helper functions
% If you want to actually use these functions within your function
% implementation, just copy and paste the function into your file
data_table = load_csv_file('detectWalking/dataset/rachel_walking_3mph_left.csv');
figure
plot(data_table.ank_ang)
%added labels
xlabel('samples')
ylabel('ankle angle (100xdeg)')

steps = get_step_cycle_data_from_data_table(data_table);
average_step = get_average_step(steps);
figure
plot(linspace(1,100,size(average_step,1)),average_step.ank_ang);
xlabel('percent of gait cycle')
ylabel('ankle angle (100xdeg)')

figure
plot_variable_by_step(data_table,'ank_ang');

ank_ang_index = variable_name_to_column_index('ank_ang');

%%
% loads a csv file into an N x M table array
% variables in the resulting table can be accessed through dot indexing the
% variable name (e.g. data_table.step_time)
% N = number of samples
% M = number of variables
function data_table = load_csv_file(file_path)
    %one line function, not really much to change
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
    %plots heel strikes inclusively; doesn't plot transition from stand to
    %walking, or walking to stand
    steps = cell((size(HS,1)-1),1);

    for k = 1:(size(HS,1)-1)
        %double counts beginning and end of each step. could be desirable
        %but i'll get rid of the beginning of the next phase for clarity
        %OLD
        %cycle = data_table(HS(k):HS(k+1),:);
        %NEW
        cycle = data_table(HS(k):HS(k+1)-1,:);
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
    %selects for the max size of a step array
    for k = 1:(size(steps,1))
        j = size(steps{k},1);
        if j > I
            I = j;
        end
    end

    variableNames = steps{1}.Properties.VariableNames;
    average_step = zeros(I,size(steps{1},2));

    for m = 1:size(steps{1},2) %iterates across variables
        data = zeros(I,1);
        for k = 1:(size(steps,1)) %iterates across steps
              %original doesn't account for shorter steps; averaged along time instead of phase 
%             for j = 1:size(steps{k},1) 
%                 data(j,1) = data(j,1) + steps{k}{j,m};
%             end

              %expand every vector to size I using linear interpolation, so
              %we average across gait phase instead of time
              if(size(steps{k},1) < I)
                  old_space = linspace(1,100,size(steps{k},1));
                  new_space = linspace(1,100,I);
                  interpolated_step = interp1(old_space,steps{k}{:,m},new_space,'spline')';
              else
                  interpolated_step = steps{k}{:,m};
              end
              data(:,1) = data(:,1) + interpolated_step;
        end
        data = data/(size(steps,1));
        %protect gait state and movement from not making sense
        if(strcmp(steps{1}.Properties.VariableNames{m},'gait_state') || ...
                strcmp(steps{1}.Properties.VariableNames{m},'movement'))
            data = round(data);
        end
        average_step(:,m) = data;
        
        %not adjusting state time since it doesn't really make sense as a
        %variable for the average step, without just making it the same as
        %step time; same for step count
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
    xlabel('samples')
    ylabel(variable,'Interpreter', 'none')
    
    
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

% returns the column index of the given variable name in a data_array,
% assuming the data array is in the same general format/order as the data in the csv logs
function variable_index = variable_name_to_column_index(variable_name)
	possible_variables = {'state_time','accelx','accely','accelz','gyrox','gyroy','gyroz','mot_ang','mot_vel','mot_acc','mot_cur','mot_volt','batt_volt','batt_curr','temperature','status_mn','status_ex','status_re','ank_ang','ank_vel','ank_torque','peak_ank_torque','step_energy','step_count','step_time','gait_state','movement','speed','incline','sys_time','event flags'};

	variable_index = -1;

    for i = 1:length(possible_variables)
        if strcmp(possible_variables{i},variable_name) == 1
			variable_index = i;
        end
    end
end
