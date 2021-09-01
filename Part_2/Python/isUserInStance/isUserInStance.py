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
Predicts when the user is in stance (i.e. has their foot on the ground)

Input:
data_array >> an N x M numpy ndarray of variables collected from a single user
while doing a single movement for multiple steps (i.e. 10 walking steps,
10 running steps, etc).

Output:
isUserInStancePredictions >> an N x 1 array, where each element in the array
represents whether or not the column in data_array representing gait_state will have a value of 1
(i.e. the method should predict a value of 1 when data_array[n,hf.variable_name_to_column_index('gait_state')] = 1 (i.e. stance)
and should predict a value of 0 when data_array[n,hf.variable_name_to_column_index('gait_state')] != 1.
"""


def isUserInStance(data_array):
	# This is a placeholder so the code will run. Replace this with your
	# implementation.
	isUserInStancePredictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("gait_state")]
	)
	return isUserInStancePredictions
