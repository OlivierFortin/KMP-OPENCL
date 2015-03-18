/****
* Follow this code : http://www.thebigblob.com/using-the-cpp-bindings-for-opencl/
* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>  
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

#ifdef __APPLE__
#include "opencl.hpp"
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)

int main(void) {

    ifstream inputFile, automateFile, failureTableFile;
    
    std::string readLine;

    //Read input File
    vector<int> input;
    inputFile.open("Data/Data2/inputFile.csv");
    while(!inputFile.eof()) {
        int value;
        char delimeter;
        inputFile >> value;
        inputFile >> delimeter;
        input.push_back(value);
             
    }
    inputFile.close();

    //Read Automate File
    std::vector<int> automate;
    automateFile.open("Data/Data2/Automate.csv");
    while(!automateFile.eof()) {
        int value;
        char delimeter;
        automateFile >> value;
        automateFile >> delimeter;
        automate.push_back(value);  
    }
    automateFile.close();

    //Read Failure table file File
    std::vector<int> failureTable;
    failureTableFile.open("Data/Data2/FailTable.csv");
    while(!failureTableFile.eof()) {
        int value;
        char delimeter;
        failureTableFile >> value;
        failureTableFile >> delimeter;
        failureTable.push_back(value);   
    }
    failureTableFile.close();


    
    //Load opencl programme
    ifstream sourceFile("opencl/base.cl");
    string opencl_source (std::istreambuf_iterator<char>(sourceFile),(std::istreambuf_iterator<char>()));


    // Get platform and device information
    cl_uint nplatforms;
    cl_platform_id* platforms;
    cl_platform_id platform;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    char buffer[256];
    int i,j;
    int err;

    cl_int ret = clGetPlatformIDs( 0,0,&nplatforms);
    platforms = (cl_platform_id*)malloc(nplatforms*sizeof(cl_platform_id));
    clGetPlatformIDs( nplatforms, platforms, 0);

    for(i=0; i<nplatforms; i++) {
      platform = platforms[i];
      clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,256,buffer,0);
      if (!strcmp(buffer,"coprthr")) break;
    }
    cout << nplatforms << "()"<< i <<"test\n";
   if (i<nplatforms) platform = platforms[i];
   else exit(1);

    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 0, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);


    cl_mem d_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, input.size(), NULL, &ret);


    // Create a program from the kernel source
    // 
    const char *source_str = opencl_source.c_str();
    size_t source_size = opencl_source.size()+1;
    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&source_str, (const size_t *)&source_size,&ret);
    cout << CL_INVALID_CONTEXT << '-' << CL_OUT_OF_HOST_MEMORY << '\n';
    cout << ret;
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "find_pattern", &ret);
    
    // Set the arguments of the kernel
    // 
    // __global int *h_input,
    // __global int *h_automate,
    // __global int *h_failureTable,
    // __global int *pattern_found){
    int patternFound = 0;
    

    ret = clSetKernelArg(kernel, 0, sizeof(int), (void *)input.size());
    
    ret = clSetKernelArg(kernel, 1, sizeof(int), (void *)input.size());
    ret = clSetKernelArg(kernel, 2, sizeof(int), (void *)automate.size());
    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&input[0]);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&automate[0]);
    ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&failureTable[0]);
    ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&patternFound);


    // ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&d_StatesTab);
    // ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&d_C);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = 1024; // Process the entire lists
    size_t local_item_size = 32; // Process in groups of 64
    clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
}