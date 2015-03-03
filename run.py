#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from numpy import genfromtxt
import pyopencl as cl
import csv
import pyopencl.array as pycl_array 

a_np = np.random.rand(50).astype(np.float32)
b_np = np.random.rand(50).astype(np.float32)

opencl_apps = open('opencl/basicTest.cl', 'r').read()

# load all csv file
# input_csv




input_csv = genfromtxt('Data/Data2/inputFile.csv', delimiter=',').astype(np.int32)
states_automate_csv = genfromtxt('Data/Data2/Automate.csv', delimiter=',').astype(np.int32)
# alphabets_csv = genfromtxt('Data/Data1/ListAlphabet.csv', delimiter='\n').astype(np.str_)
# output_states_csv = genfromtxt('Data/Data1/OutputStates.csv', delimiter='\n').astype(np.int32)
# states_tab_csv = genfromtxt('Data/Data1/StatesTab.csv', delimiter=';').astype(np.int32)
# print states_tab_csv

# Create Context and queue
ctx = cl.create_some_context();
queue = cl.CommandQueue(ctx)

mf = cl.mem_flags
res_input = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=input_csv)
res_automate = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=states_automate_csv)


prg = cl.Program(ctx, opencl_apps).build()
prg.CheckMatch(queue, (16,), None, np.int32(72), np.int32(128), res_input, res_states_tab)






