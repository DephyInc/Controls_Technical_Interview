[parentdir,~,~] = fileparts(pwd);
addpath(parentdir);

%% Here's an example of the ideal output for a given input
data_table = readtable('dataset/chenlu_walking_3mph_left.csv');
examplePeakTorquePredictions = examplePredictPeakTorque(data_table);

function examplePeakTorquePredictions = examplePredictPeakTorque(data_table)
    
    examplePeakTorquePredictions = zeros(size(data_table.ank_torque));
    
    HS = getHSindices(data_table);
    steps = get_step_cycle_data_from_data_table(data_table);
    get_step_cycle_data_from_data_table
    
    j = HS(1,1);
    for k = 1:(size(steps,1))
        [m,i] = max(steps{k}{:,'ank_torque'});
        examplePeakTorquePredictions(j+i-1,1) = m;
        j = j+size(steps{k},1);
    end
    
 
    figure
    plot(data_table.ank_torque, 'b')
    hold on;
    plot(examplePeakTorquePredictions,'r')
end