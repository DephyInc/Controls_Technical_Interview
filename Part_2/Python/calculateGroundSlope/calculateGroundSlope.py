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
Predicts the slope of the ground as a % grade

Input:
data_array >> an N x M numpy array of variables collected from a single user
while doing a single movement for multiple steps (i.e. 10 walking steps,
10 running steps, etc).

Output:
groundSlopePredictions >> an N x 1 array, where each element in the array
represents the % grade of the ground the user is currently moving over
(i.e. the method should be a rough prediction of the value of
data_array[n,hf.variable_name_to_column_index('incline')],
though hopefully your prediction is much better than ours)
"""


def calculateGroundSlope(data_array):
	# This is a placeholder so the code will run. Replace this with your
	# implementation.
	groundSlopePredictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("incline")]
	)
	return groundSlopePredictions
