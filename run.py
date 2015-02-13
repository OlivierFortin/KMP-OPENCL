#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import pyopencl as cl
import csv

a_np = np.random.rand(50).astype(np.float32)
b_np = np.random.rand(50).astype(np.float32)

opencl_apps = open('opencl/basicTest.cl', 'r').read()

# load all csv file
#StatesTab.csv
states_file = open('Data/Data1/StatesTab.csv')
state_csv = csv.reader(states_file)

# for row in state_csv:
#   print state_csv
#   


ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)

mf = cl.mem_flags
a_g = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=a_np)
b_g = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=b_np)


prg = cl.Program(ctx, opencl_apps).build()

res_g = cl.Buffer(ctx, mf.WRITE_ONLY, a_np.nbytes)
prg.sum(queue, a_np.shape, None, a_g, b_g, res_g)

res_np = np.empty_like(a_np)
cl.enqueue_copy(queue, res_np, res_g)

# Check on CPU with Numpy:
print(res_np - (a_np + b_np))
print(np.linalg.norm(res_np - (a_np + b_np)))