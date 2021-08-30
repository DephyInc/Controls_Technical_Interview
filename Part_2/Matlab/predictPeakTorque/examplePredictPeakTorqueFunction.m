%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/chenlu_walking_3mph_left.csv');
examplePeakTorquePredictions = examplePredictPeakTorque(data_table);

function examplePeakTorquePredictions = examplePredictPeakTorque(data_table)
    
    examplePeakTorquePredictions = zeros(size(data_table.ank_torque));
    
    HS = getHSindices(data_table);
    steps = get_step_cycle_data_from_data_table(data_table);
    
    j = HS(1,1)-1;
    for k = 1:(size(steps,1))
        [m,i] = max(steps{k}{:,'ank_torque'});
        examplePeakTorquePredictions(j+i,1) = m;
        j = j+size(steps{k},1)-1;
    end
    
    figure
    plot(data_table.ank_torque, 'b')
    hold on;
    plot(examplePeakTorquePredictions,'r')
end

%% Helper functions taken from the helper function file

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