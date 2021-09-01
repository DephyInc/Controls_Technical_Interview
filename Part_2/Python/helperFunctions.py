#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as mplt

np.warnings.filterwarnings("ignore", category=np.VisibleDeprecationWarning)

# Run the file for an example of how to use the helper functions
# If you want to actually use these functions within your function
# implementation, either:
# 1) just copy and paste the function into your file
# OR
# 2) import the helper functions file using the following, or something similar
# import os, sys
# currentdir = os.path.dirname(os.path.realpath(__file__))
# parentdir = os.path.dirname(currentdir)
# sys.path.append(parentdir)
# import helperFunctions as hf


def main():
	data_array = load_csv_file("detectWalking/dataset/rachel_walking_3mph_left.csv")
	mplt.figure(1)
	mplt.plot(data_array[:, variable_name_to_column_index("ank_ang")])
	mplt.draw()

	steps = get_step_cycle_data_from_data_table(data_array)
	average_step = get_average_step(steps)
	mplt.figure(2)
	mplt.plot(average_step[:, variable_name_to_column_index("ank_ang")])
	mplt.draw()

	mplt.figure(3)
	plot_variable_by_step(data_array, "ank_ang")
	mplt.draw()

	mplt.show()


"""
loads a csv file into an N x M numpy ndarray
variables in the resulting array can be accessed through bracket indexing the
column of the variable you're interested in (e.g. data_array[:,33])
N = number of samples
M = number of variables
"""


def load_csv_file(file_path):
	data_array = np.genfromtxt(file_path, delimiter=",", skip_header=1)
	return data_array


"""
turns an N x M data array of a user doing some repeated movement into a
K element list of step data (i.e. presumably the data we've provided
you is somewhat periodic. this function allows you to break the data down
and see what's happening in each period)
each element of the list will be a J x M numpy ndarray
variables from step k of the list can be accessed through bracket indexing
(e.g. steps[k][:,33])
K = number of steps
J = number of samples that occured in this step
M = number of variables
"""


def get_step_cycle_data_from_data_table(data_array):
	HS = getHSindices(data_array)

	steps = [0] * (np.size(HS, 0) - 1)

	for k in range(0, np.size(HS, 0) - 1):
		cycle = data_array[HS[k] : HS[k + 1], :]
		steps[k] = cycle

	return steps


"""
takes K element list of step data (each element a J x M ndarray) and averages each
variable along the K and M axes, such that the resulting I x M numpy ndarray is an
average model of the input steps
variables in the resulting array can be accessed through bracket indexing the
column of the variable you're interested in (e.g. average_step[:,33])
"""


def get_average_step(steps):
	I = 0
	for k in range(0, np.size(steps, 0)):
		j = np.size(steps[k], 0)
		if j > I:
			I = j

	average_step = np.zeros((I, np.size(steps[1], 1)))

	for m in range(0, np.size(steps[1], 1)):
		data = np.zeros((I, 1))
		for k in range(0, np.size(steps, 0)):
			for j in range(0, np.size(steps[k], 0)):
				data[j, 0] = data[j, 0] + steps[k][j, m]

		data = data / (np.size(steps, 0))
		average_step[:, m] = data[:, 0]

	return average_step


"""
plots a given variable by step (i.e. presumably the data we've provided
you is somewhat periodic. this function allows you to break the data down
and plot each individual period)
note: variable should be a string
"""


def plot_variable_by_step(data_array, variable):
	HS = getHSindices(data_array)

	steps = [0] * (np.size(HS, 0) - 1)

	for k in range(0, np.size(HS, 0) - 1):
		cycle = data_array[HS[k] : HS[k + 1], :]
		steps[k] = cycle

	for k in range(0, len(steps)):
		mplt.plot(steps[k][:, variable_name_to_column_index(variable)])


"""
returns the indices of the rows of the data table in which a heelstrike occurs
(heelstrike is when your foot first touches the ground at the beginning
of a step)
"""


def getHSindices(data_array):
	HS = DetectPosZC(0, data_array[:, variable_name_to_column_index("gait_state")])

	return HS


def DetectPosZC(offset, data):
	ZC = []
	for i in range(1, len(data)):
		if (data[i] > offset) and (data[i - 1] <= offset):
			ZC = np.append(ZC, i)

	ZC = ZC.astype("int64")

	return ZC


"""
returns the column index of the given variable name in a data_array,
assuming the data array is in the same general format/order as the data in the csv logs
"""


def variable_name_to_column_index(variable_name):
	possible_variables = [
		"state_time",
		"accelx",
		"accely",
		"accelz",
		"gyrox",
		"gyroy",
		"gyroz",
		"mot_ang",
		"mot_vel",
		"mot_acc",
		"mot_cur",
		"mot_volt",
		"batt_volt",
		"batt_curr",
		"temperature",
		"status_mn",
		"status_ex",
		"status_re",
		"ank_ang",
		"ank_vel",
		"ank_torque",
		"peak_ank_torque",
		"step_energy",
		"step_count",
		"step_time",
		"gait_state",
		"movement",
		"speed",
		"incline",
		"sys_time",
		"event flags",
	]

	variable_index = 0

	for variable in possible_variables:
		if variable == variable_name:
			return variable_index

		variable_index += 1

	return -1


if __name__ == "__main__":
	main()
