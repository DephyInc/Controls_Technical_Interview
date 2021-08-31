#!/usr/bin/env python
from copy import deepcopy
import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
import helperFunctions as hf

'''
Implement the following function

Description:
Predicts when the user is walking

Input:
data_array >> an N x M numpy array of variables collected from a single user
while doing a single movement for multiple steps (i.e. 10 walking steps, 
10 running steps, etc).

Output:
detectWalkingPredictions >> an N x 1 array, where each element in the array
represents whether or not the column in data_array representing movement will have a value of 1 (i.e. walking)
IMPORTANT NOTE: The minimum implementation should predict a value of 1 when 
data_array[n,hf.variable_name_to_column_index('movement')] = 1 and should predict 
a value of 0 when data_array[n,hf.variable_name_to_column_index('movement')] != 1.
An optional, more advanced implementation would simply predict the value
of data_array[n,hf.variable_name_to_column_index('movement')].
'''

def detectWalking(data_array):
	# This is a placeholder so the code will run. Replace this with your
	# implementation.
	detectWalkingPredictions = deepcopy(data_array[:,hf.variable_name_to_column_index('movement')])
	return detectWalkingPredictions