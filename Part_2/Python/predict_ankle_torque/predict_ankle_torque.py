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
ankle_torque_predictions >> an N x 1 array, where each element in the array
represents how much ankle torque the user would like to receive at this point in the step
(i.e. this function should predict the value of the column in data_array representing ankle torque)
"""

#!/usr/bin/env python
from copy import deepcopy
from .. import helper_functions as hf


def predict_ankle_torque(data_array):
	"""
	This is a placeholder so the code will run. Replace this with your
	implementation.
	"""
	ankle_torque_predictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("ank_torque")]
	)
	return ankle_torque_predictions
