#!/usr/bin/env python
import os
import matplotlib.pyplot as mplt
from . import calculate_ground_slope as cgs
from .. import helper_functions as hf


def test_calculate_ground_slope():
	"""Test your code by running the following"""
	files = os.listdir("dataset")

	i = 0
	for filename in files:
		data_array = hf.load_csv_file("dataset/" + filename)

		ground_slope_predictions = cgs.calculate_ground_slope(data_array)

		mplt.figure(i + 1)
		mplt.plot(data_array[:, hf.variable_name_to_column_index("incline")], "b")
		mplt.plot(ground_slope_predictions, "r")
		mplt.draw()

		i += 1

	mplt.show()


if __name__ == "__main__":
	test_calculate_ground_slope()
