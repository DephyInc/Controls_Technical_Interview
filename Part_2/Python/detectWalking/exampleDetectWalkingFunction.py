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

def exampleDetectWalking(data_array):
	exampleDetectWalkingPredictions = deepcopy(data_array[:,hf.variable_name_to_column_index('movement')])

	return exampleDetectWalkingPredictions

if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	data_array = hf.load_csv_file('dataset/rachel_walking_3mph_left.csv')
	exampleDetectWalkingPredictions = exampleDetectWalking(data_array)

	mplt.figure(1)
	mplt.plot(data_array[:,hf.variable_name_to_column_index('movement')], 'b')
	mplt.plot(exampleDetectWalkingPredictions,'r')
	mplt.show()