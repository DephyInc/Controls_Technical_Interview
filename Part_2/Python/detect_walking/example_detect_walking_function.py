#!/usr/bin/env python
from copy import deepcopy
import numpy as np
import matplotlib.pyplot as mplt
from .. import helper_functions as hf

np.warnings.filterwarnings("ignore", category=np.VisibleDeprecationWarning)


def example_detect_walking(data_array):
	example_detect_walking_predictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("movement")]
	)

	return example_detect_walking_predictions


if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	DATA_ARRAY = hf.load_csv_file("dataset/rachel_walking_3mph_left.csv")
	EXAMPLE_DETECT_WALKING_PREDICTIONS = example_detect_walking(DATA_ARRAY)

	mplt.figure(1)
	mplt.plot(DATA_ARRAY[:, hf.variable_name_to_column_index("movement")], "b")
	mplt.plot(EXAMPLE_DETECT_WALKING_PREDICTIONS, "r")
	mplt.show()
