#!/usr/bin/env python
import os
import matplotlib.pyplot as mplt
from . import is_user_in_stance as iuis
from .. import helper_functions as hf


def test_is_user_in_stance():
	"""Test your code with this function"""
	files = os.listdir("dataset")

	i = 0
	for filename in files:
		data_array = hf.load_csv_file("dataset/" + filename)

		is_user_in_stance_predictions = iuis.is_user_in_stance(data_array)

		mplt.figure(i + 1)
		mplt.plot(data_array[:, hf.variable_name_to_column_index("gait_state")], "b")
		mplt.plot(is_user_in_stance_predictions, "r")
		mplt.draw()

		i += 1

	mplt.show()


if __name__ == "__main__":
	test_is_user_in_stance()
