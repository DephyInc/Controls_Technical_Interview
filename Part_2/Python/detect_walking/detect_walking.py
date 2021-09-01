"""
Implement the following function

Description:
Predicts when the user is walking

Input:
data_array >> an N x M numpy array of variables collected from a single user
while doing a single movement for multiple steps (i.e. 10 walking steps,
10 running steps, etc).

Output:
detect_walking_predictions >> an N x 1 array, where each element in the array
represents whether or not the column in data_array representing movement will have a value of 1 (i.e. walking)
IMPORTANT NOTE: The minimum implementation should predict a value of 1 when
data_array[n,hf.variable_name_to_column_index('movement')] = 1 and should predict
a value of 0 when data_array[n,hf.variable_name_to_column_index('movement')] != 1.
An optional, more advanced implementation would simply predict the value
of data_array[n,hf.variable_name_to_column_index('movement')].
"""
#!/usr/bin/env python
from copy import deepcopy
from .. import helper_functions as hf


def detect_walking(data_array):
	"""
	This is a placeholder so the code will run. Replace this with your
	implementation.
	"""
	detect_walking_predictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("movement")]
	)
	return detect_walking_predictions
