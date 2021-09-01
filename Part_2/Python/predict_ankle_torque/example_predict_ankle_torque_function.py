#!/usr/bin/env python
"""Example script"""
import os
import sys
from copy import deepcopy
import numpy as np
import matplotlib.pyplot as mplt

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
import helper_functions as hf  # pylint: disable=wrong-import-position disable=import-error

np.warnings.filterwarnings("ignore", category=np.VisibleDeprecationWarning)


def example_predict_ankle_torque(data_array):
	"""Run example function"""
	example_ankle_torque_predictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("ank_torque")]
	)

	return example_ankle_torque_predictions


if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	DATA_ARRAY = hf.load_csv_file("dataset/chenlu_walking_3mph_left.csv")
	EXAMPLE_ANKLE_TORQUE_PREDICTIONS = example_predict_ankle_torque(DATA_ARRAY)

	mplt.figure(1)
	mplt.plot(DATA_ARRAY[:, hf.variable_name_to_column_index("ank_torque")], "b")
	mplt.plot(EXAMPLE_ANKLE_TORQUE_PREDICTIONS, "r")
	mplt.show()
