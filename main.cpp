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



#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)

int main(void) {

    ifstream inputFile, automateFile;
    
    std::string readLine;

    //Read input File
    std::vector<int> input;
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
        cout << value;      
    }
    automateFile.close();
    
    //Load the OpenCl file
	

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
}