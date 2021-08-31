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

def exampleCalculateGroundSlope(data_array):
	exampleGroundSlopePredictions = np.zeros(np.size(data_array,0))

	return exampleGroundSlopePredictions

if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	data_array = hf.load_csv_file('dataset/nick_walking_0incline_left.csv')
	exampleGroundSlopePredictions = exampleCalculateGroundSlope(data_array)

	mplt.figure(1)
	mplt.plot(data_array[:,hf.variable_name_to_column_index('incline')], 'b')
	mplt.plot(exampleGroundSlopePredictions,'r')
	mplt.show()