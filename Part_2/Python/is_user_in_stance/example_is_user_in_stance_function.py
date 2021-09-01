#!/usr/bin/env python
from copy import deepcopy
import numpy as np
import matplotlib.pyplot as mplt
from .. import helper_functions as hf

np.warnings.filterwarnings("ignore", category=np.VisibleDeprecationWarning)


def example_is_user_in_stance(data_array):
	example_is_user_in_stance_predictions = deepcopy(
		data_array[:, hf.variable_name_to_column_index("gait_state")]
	)
	example_is_user_in_stance_predictions[example_is_user_in_stance_predictions < 0] = 0

	return example_is_user_in_stance_predictions


if __name__ == "__main__":
	# Here's an example of the ideal output for a given input
	DATA_ARRAY = hf.load_csv_file("dataset/rachel_walking_3mph_left.csv")
	EXAMPLE_IS_USER_IN_STANCE_PREDICTIONS = example_is_user_in_stance(DATA_ARRAY)

	mplt.figure(1)
	mplt.plot(DATA_ARRAY[:, hf.variable_name_to_column_index("gait_state")], "b")
	mplt.plot(EXAMPLE_IS_USER_IN_STANCE_PREDICTIONS, "r")
	mplt.show()
