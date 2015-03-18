import numpy as np
from numpy import genfromtxt
import csv
import pyopencl.array as pycl_array 

# from wikipedia algo
# Modify to use np array
def partial(W):
	T = np.zeros(len(W)).astype(np.int32)
	T[0] = -1
	T[1] = 0
	pos = 2
	cnd = 0

	while (pos < len(W)):
		if W[pos-1] == W[cnd]:
			cnd +=1
			T[pos] = cnd
			pos += 1
		elif cnd > 0:
			cnd = T[cnd]
		else:
			T[pos] = 0
			pos += 1
	return T


with open('Data/Data2/Automate.csv', 'rb') as csvfile: 
	automate = csv.reader(csvfile, delimiter=',')
	automate_str = ""
	for row in automate:
		automate_str = automate_str.join(row)
	failureTable = partial(automate_str)
	print failureTable
