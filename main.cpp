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
#define DEVICE_TYPE CL_DEVICE_TYPE_CPU


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


  int i,j;
   int err;
   char buffer[256];

   unsigned int n = 1024;

   cl_uint nplatforms;
   cl_platform_id* platforms;
   cl_platform_id platform;


   //---------------------------------------------------------
   //Discover and initialize the platform
   //--------------------------------------------------------- 
   clGetPlatformIDs( 0,0,&nplatforms);
   platforms = (cl_platform_id*)malloc(nplatforms*sizeof(cl_platform_id));
   clGetPlatformIDs( nplatforms, platforms, 0);

   for(i=0; i<nplatforms; i++) {
      platform = platforms[i];
      clGetPlatformInfo(platforms[i],CL_PLATFORM_NAME,256,buffer,0);
      cout << buffer;
      if (!strcmp(buffer,"coprthr")) break;
   }

   if (i<nplatforms) platform = platforms[i];
   else platform = platforms[0];
   //---------------------------------------------------------
   //Discover and initialize the devices 
   //--------------------------------------------------------- 
   cl_uint ndevices;
   cl_device_id* devices;
   cl_device_id dev;

   clGetDeviceIDs(platform,DEVICE_TYPE,0,0,&ndevices);
   devices = (cl_device_id*)malloc(ndevices*sizeof(cl_device_id));
   clGetDeviceIDs(platform, DEVICE_TYPE,ndevices,devices,0);

   dev = devices[0];
   


   //---------------------------------------------------------
   //Create a context
   //---------------------------------------------------------    
   cl_context_properties ctxprop[3] = {
      (cl_context_properties)CL_CONTEXT_PLATFORM,
      (cl_context_properties)platform,
      (cl_context_properties)0
   };
   cl_context ctx = clCreateContext(ctxprop,1,&dev,0,0,&err);

   //---------------------------------------------------------
   //Create a command queue
   //---------------------------------------------------------    
   cl_command_queue cmdq = clCreateCommandQueue(ctx,dev,0,&err);

   //---------------------------------------------------------
   //The kernel
   //---------------------------------------------------------   
   const char kernel_code[] = 
      "__kernel void find_pattern(int inputL,"
        "                        int  h_input_length,"
        "                        int h_automate_length,"
        "                       __global int *h_input,"
        "                       __global int *h_automate,"
        "                       __global int *h_failureTable,"
        "                       __global int *pattern_found)\n"
        "{\n"
        "   printf(\"test opencl \");\n"
        "   int gid = get_global_id(0);\n"
        "   int m = gid*inputL;//position in input\n"
        "   int i = 0;//position in automate\n"
        "   int counter = 0;\n"
        "   printf(\"Before while \");\n"
        "    while (*pattern_found != 1 & counter < inputL+h_automate_length  & m+i < h_input_length) {\n"
        "       if(h_automate[i] == h_input[m+i]) {\n"
        "           if (i == h_automate_length - 1 ) {\n"
        "               *pattern_found = 1;\n"
        "               printf(\"Found pattern  \");\n"
        "               return;\n"
        "           }\n"
        "           ++i;\n"
        "       } else {\n"
        "           if (h_failureTable[i] > -1) {\n"
        "               m = m + i - h_failureTable[i];\n"
        "               i = h_failureTable[i];\n"
        "           } else {\n"
        "               i = 0;\n"
        "               ++m;\n"
        "               ++counter;\n"
        "           }\n"
        "       }\n"
        "    }\n"
        "}\n";



   cl_mem input_buf = clCreateBuffer(ctx,CL_MEM_USE_HOST_PTR,input.size()*sizeof(int),&input[0],&err);
   cl_mem automate_buf = clCreateBuffer(ctx,CL_MEM_USE_HOST_PTR,input.size()*sizeof(int),&automate[0],&err);
   cl_mem failuretable_buf = clCreateBuffer(ctx,CL_MEM_USE_HOST_PTR,automate.size()*sizeof(int),&failureTable[0],&err);

   //---------------------------------------------------------
   //Compiling the kernel
   //---------------------------------------------------------   
   const char* src[1] = { kernel_code };
   size_t src_sz = sizeof(kernel_code);

   cl_program prg = clCreateProgramWithSource(ctx,1,(const char**)&src,
        &src_sz,&err);

    clBuildProgram(prg,1,&dev,0,0,0);

    cl_kernel krn = clCreateKernel(prg,"find_pattern",&err);

    int ret;
    int pattern_found = 1;

    ret = clSetKernelArg(krn, 0, sizeof(int), (void *)input.size()/32);
    ret = clSetKernelArg(krn, 1, sizeof(cl_int), (void *)input.size());
    ret = clSetKernelArg(krn, 2, sizeof(cl_int), (void *)automate.size());
    ret = clSetKernelArg(krn, 3, sizeof(cl_mem), (void *)&input_buf);
    ret = clSetKernelArg(krn, 4, sizeof(cl_mem), (void *)&automate_buf);
    ret = clSetKernelArg(krn, 5, sizeof(cl_mem), (void *)&failuretable_buf);
    ret = clSetKernelArg(krn, 6, sizeof(cl_int), (void *)&pattern_found);

    size_t gtdsz[] = { 1 };
    size_t ltdsz[] = { 1 };
    cl_event ev[10];

    clEnqueueNDRangeKernel(cmdq,krn,1,0,gtdsz,ltdsz,0,NULL,NULL);



}