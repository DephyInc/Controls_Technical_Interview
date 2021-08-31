#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as mplt
import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
import helperFunctions as hf
np.warnings.filterwarnings('ignore', category=np.VisibleDeprecationWarning)

def examplePredictAnkleTorque(data_array):
	exampleAnkleTorquePredictions = data_array[:,hf.variable_name_to_column_index('ank_torque')]

	return exampleAnkleTorquePredictions

if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	data_array = hf.load_csv_file('dataset/chenlu_walking_3mph_left.csv')
	exampleAnkleTorquePredictions = examplePredictAnkleTorque(data_array)

	mplt.figure(1)
	mplt.plot(data_array[:,hf.variable_name_to_column_index('ank_torque')], 'b')
	mplt.plot(exampleAnkleTorquePredictions,'r')
	mplt.show()