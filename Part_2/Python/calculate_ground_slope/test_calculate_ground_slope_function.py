#!/usr/bin/env python
"""Test script"""
import os
import sys
import matplotlib.pyplot as mplt

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
# pylint: disable=wrong-import-position disable=import-error
import calculate_ground_slope as cgs
import helper_functions as hf


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
