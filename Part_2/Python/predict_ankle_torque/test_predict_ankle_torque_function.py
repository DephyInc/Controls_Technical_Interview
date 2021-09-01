#!/usr/bin/env python
import os
import matplotlib.pyplot as mplt
from . import predict_ankle_torque as pat
from .. import helper_functions as hf


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
