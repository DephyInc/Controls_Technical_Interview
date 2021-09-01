"""
Implement the following function

Description:
Predicts when the user is in stance (i.e. has their foot on the ground)

Input:
data_array >> an N x M numpy ndarray of variables collected from a single user
while doing a single movement for multiple steps (i.e. 10 walking steps,
10 running steps, etc).

Output:
is_user_in_stance_predictions >> an N x 1 array, where each element in the array
represents whether or not the column in data_array representing gait_state will have a value of 1
(i.e. the method should predict a value of 1 when data_array[n,hf.variable_name_to_column_index('gait_state')] = 1 (i.e. stance)
and should predict a value of 0 when data_array[n,hf.variable_name_to_column_index('gait_state')] != 1.
"""
#!/usr/bin/env python
from copy import deepcopy
from .. import helper_functions as hf


def is_user_in_stance(data_array):
	"""
	This is a placeholder so the code will run. Replace this with your
	implementation.
	"""
	is_user_in_stance_predictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("gait_state")]
	)
	return is_user_in_stance_predictions
