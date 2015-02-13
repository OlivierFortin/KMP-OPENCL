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




input_csv = genfromtxt('Data/Data1/inputFile.csv', delimiter='\n').astype(np.int32)
print input_csv
print input_csv.shape
print input_csv.data

alphabets_csv = genfromtxt('Data/Data1/ListAlphabet.csv', delimiter='\n').astype(np.str_)
#print alphabets_csv
output_states_csv = genfromtxt('Data/Data1/OutputStates.csv', delimiter='\n').astype(np.int32)
#print output_states_csv
states_tab_csv = genfromtxt('Data/Data1/StatesTab.csv', delimiter=';').astype(np.int32)
#print states_tab_csv
# # Create the OpenCl context
# ctx = cl.create_some_context()
# queue = cl.CommandQueue(ctx)

# mf = cl.mem_flags
# a_g = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=a_np)
# b_g = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=b_np)


# prg = cl.Program(ctx, opencl_apps).build()

# res_g = cl.Buffer(ctx, mf.WRITE_ONLY, a_np.nbytes)
# prg.sum(queue, a_np.shape, None, a_g, b_g, res_g)

# res_np = np.empty_like(a_np)
# cl.enqueue_copy(queue, res_np, res_g)

# # Check on CPU with Numpy:
# print(res_np - (a_np + b_np))
# print(np.linalg.norm(res_np - (a_np + b_np)))

# Create Context and queue
ctx = cl.create_some_context();
queue = cl.CommandQueue(ctx)

mf = cl.mem_flags
res_input = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=input_csv)
res_states_tab = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=states_tab_csv)
# res_alphabets = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=alphabets_csv)
# res_output_states = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=output_states_csv)
# 
# 



prg = cl.Program(ctx, opencl_apps).build()
prg.CheckMatch(queue, input_csv.shape, None, res_input)






