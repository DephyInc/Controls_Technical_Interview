#!/usr/bin/env python
"""Test script"""
import os
import sys
import matplotlib.pyplot as mplt

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
# pylint: disable=wrong-import-position disable=import-error
import predict_ankle_torque as pat
import helper_functions as hf


def test_predict_ankle_torque():
	"""Test your code with this function"""
	files = os.listdir("dataset")

	i = 0
	for filename in files:
		data_array = hf.load_csv_file("dataset/" + filename)

		ankle_torque_predictions = pat.predict_ankle_torque(data_array)

		mplt.figure(i + 1)
		mplt.plot(data_array[:, hf.variable_name_to_column_index("ank_torque")], "b")
		mplt.plot(ankle_torque_predictions, "r")
		mplt.draw()

		i += 1

	mplt.show()


if __name__ == "__main__":
	test_predict_ankle_torque()
