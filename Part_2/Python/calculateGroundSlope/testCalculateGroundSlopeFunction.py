#!/usr/bin/env python
import calculateGroundSlope as cgs
import matplotlib.pyplot as mplt
import os, sys

currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
import helperFunctions as hf

# Test your code by running the following


def testCalculateGroundSlope():
	files = os.listdir("dataset")

	i = 0
	for filename in files:
		data_array = hf.load_csv_file("dataset/" + filename)

		groundSlopePredictions = cgs.calculateGroundSlope(data_array)

		mplt.figure(i + 1)
		mplt.plot(data_array[:, hf.variable_name_to_column_index("incline")], "b")
		mplt.plot(groundSlopePredictions, "r")
		mplt.draw()

		i += 1

	mplt.show()


if __name__ == "__main__":
	testCalculateGroundSlope()
