#!/usr/bin/env python
"""Test script"""
import os
import sys
import matplotlib.pyplot as mplt

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
# pylint: disable=wrong-import-position disable=import-error
import detect_walking as dw
import helper_functions as hf


def test_detect_walking():
	"""Test your code with this functions"""
	files = os.listdir("dataset")

	i = 0
	for filename in files:
		data_array = hf.load_csv_file("dataset/" + filename)

		detect_walking_predictions = dw.detect_walking(data_array)

		mplt.figure(i + 1)
		mplt.plot(data_array[:, hf.variable_name_to_column_index("movement")], "b")
		mplt.plot(detect_walking_predictions, "r")
		mplt.draw()

		i += 1

	mplt.show()


if __name__ == "__main__":
	test_detect_walking()
