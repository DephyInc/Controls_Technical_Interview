#!/usr/bin/env python
"""Example script"""
import os
import sys
import numpy as np
import matplotlib.pyplot as mplt

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
import helper_functions as hf  # pylint: disable=wrong-import-position disable=import-error

np.warnings.filterwarnings("ignore", category=np.VisibleDeprecationWarning)


def example_calculate_ground_slope(data_array):
	"""Run example function"""
	example_ground_slope_predictions = np.zeros(np.size(data_array, 0))

	return example_ground_slope_predictions


if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	DATA_ARRAY = hf.load_csv_file("dataset/nick_walking_0incline_left.csv")
	EXAMPLE_GROUND_SLOPE_PREDICTIONS = example_calculate_ground_slope(DATA_ARRAY)

	mplt.figure(1)
	mplt.plot(DATA_ARRAY[:, hf.variable_name_to_column_index("incline")], "b")
	mplt.plot(EXAMPLE_GROUND_SLOPE_PREDICTIONS, "r")
	mplt.show()
