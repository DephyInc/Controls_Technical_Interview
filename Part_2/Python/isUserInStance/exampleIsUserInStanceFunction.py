#!/usr/bin/env python
from copy import deepcopy
import numpy as np
import matplotlib.pyplot as mplt
import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
import helperFunctions as hf
np.warnings.filterwarnings('ignore', category=np.VisibleDeprecationWarning)

def exampleIsUserInStance(data_array):
	exampleIsUserInStancePredictions = deepcopy(data_array[:,hf.variable_name_to_column_index('gait_state')])
	exampleIsUserInStancePredictions[exampleIsUserInStancePredictions < 0] = 0

	return exampleIsUserInStancePredictions

if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	data_array = hf.load_csv_file('dataset/rachel_walking_3mph_left.csv')
	exampleIsUserInStancePredictions = exampleIsUserInStance(data_array)

	mplt.figure(1)
	mplt.plot(data_array[:,hf.variable_name_to_column_index('gait_state')], 'b')
	mplt.plot(exampleIsUserInStancePredictions,'r')
	mplt.show()