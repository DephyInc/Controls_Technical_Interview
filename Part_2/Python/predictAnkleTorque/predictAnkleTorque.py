#!/usr/bin/env python
from copy import deepcopy
import os, sys

currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
import helperFunctions as hf

"""
Implement the following function

Description:
Predicts how much ankle torque the user should recieve from the exo when
walking

Input:
data_array >> an N x M numpy ndarray of variables collected from a single user
while doing a single movement for multiple steps (i.e. 10 walking steps,
10 running steps, etc).

Output:
ankleTorquePredictions >> an N x 1 array, where each element in the array
represents how much ankle torque the user would like to receive at this point in the step
(i.e. this function should predict the value of the column in data_array representing ankle torque)
"""


def predictAnkleTorque(data_array):
	# This is a placeholder so the code will run. Replace this with your
	# implementation.
	ankleTorquePredictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("ank_torque")]
	)
	return ankleTorquePredictions
