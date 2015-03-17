#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from numpy import genfromtxt
import pyopencl as cl
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

a_np = np.random.rand(50).astype(np.float32)
b_np = np.random.rand(50).astype(np.float32)

opencl_apps = open('opencl/base.cl', 'r').read()

# load all csv file
# input_csv

input_csv = genfromtxt('Data/Data2/inputFile.csv', delimiter=',').astype(np.int32)
states_automate_csv = genfromtxt('Data/Data2/Automate.csv', delimiter=',').astype(np.int32)
test  = genfromtxt('Data/Data2/Automate.csv', delimiter=',')

failureTable  = []
with open('Data/Data2/Automate.csv', 'rb') as csvfile: 
	automate = csv.reader(csvfile, delimiter=',')
	automate_str = ""
	for row in automate:
		automate_str = automate_str.join(row)
	failureTable = partial(automate_str)

print failureTable
print 
# Create Context and queue
ctx = cl.create_some_context();
queue = cl.CommandQueue(ctx)

mf = cl.mem_flags
res_input = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=input_csv)
res_automate = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=states_automate_csv)
res_failure_table = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=failureTable)
pattern_found = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=np.int32(0))

nb_process = 4
from time import time
prg = cl.Program(ctx, opencl_apps).build()
t_start = time()
prg.find_pattern(queue, (nb_process,), None, np.int32(len(input_csv)/nb_process),np.int32(len(input_csv)),np.int32(len(states_automate_csv)), res_input, res_automate, res_failure_table,pattern_found)
t_end = time()
print 'Take : %f' %(t_end-t_start)




