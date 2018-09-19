// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class CLUtil
using namespace std;

#include <vector>
#include <iostream>

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "CLUtil.h"

cl::Device CLUtil::choose_platform_and_device(){
	// Find all available OpenCL platforms (e.g. AMD, Nvidia, Intel)
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    // Show the names of all available OpenCL platforms
    cout << "Available OpenCL platforms: \n\n";
    for (unsigned int i = 0; i < platforms.size(); i++)
        cout << "\t" << i + 1 << ": " << platforms[i].getInfo<CL_PLATFORM_NAME>() << endl;

    // Choose and create an OpenCL platform
    cout << endl << "Enter the number of the OpenCL platform you want to use: ";
    unsigned int input = 0;
    cin >> input;
    // Handle incorrect user input
    while (input < 1 || input > platforms.size()){
        cin.clear(); //clear errors/bad flags on cin
        cin.ignore(cin.rdbuf()->in_avail(), '\n'); // ignores exact number of chars in cin buffer
        cout << "No such platform." << endl << "Enter the number of the OpenCL platform you want to use: ";
        cin >> input;
    }

    cl::Platform platform = platforms[input - 1];

    // Print the name of chosen OpenCL platform
    cout << "Using OpenCL platform: \t" << platform.getInfo<CL_PLATFORM_NAME>() << endl;

    // Find all available OpenCL devices (e.g. CPU, GPU or integrated GPU)
    vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    // Print the names of all available OpenCL devices on the chosen platform
    cout << "Available OpenCL devices on this platform: " << endl << endl;
    for (unsigned int i = 0; i < devices.size(); i++)
        cout << "\t" << i + 1 << ": " << devices[i].getInfo<CL_DEVICE_NAME>() << endl;

    // Choose an OpenCL device
    cout << endl << "Enter the number of the OpenCL device you want to use: ";
    input = 0;
    cin >> input;
    // Handle incorrect user input
    while (input < 1 || input > devices.size()){
        cin.clear(); //clear errors/bad flags on cin
        cin.ignore(cin.rdbuf()->in_avail(), '\n'); // ignores exact number of chars in cin buffer
        cout << "No such device. Enter the number of the OpenCL device you want to use: ";
        cin >> input;
    }

    cl::Device device = devices[input - 1];

    // Print the name of the chosen OpenCL device
    cout << endl << "Using OpenCL device: \t" << device.getInfo<CL_DEVICE_NAME>() << endl << endl;
    return device;
}



CLSphere* CLUtil::get_cl_spheres(const World& world){
	int num_objects = world.objects.size();
	CLSphere* ret = new CLSphere[num_objects];
	for(int i = 0; i < num_objects; i ++){
		Sphere* sphere = (Sphere*) world.objects[i];
		ret[i] = sphere->get_cl_sphere();
	}
	return ret;
}

void CLUtil::attempt_build_program(cl::Program program, cl::Device device){
    // Build the program and check for compilation errors
	cl_int result = program.build({ device }, "");
	if (result == CL_BUILD_PROGRAM_FAILURE)
	{
		// Check the build status
		cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
		if (status == CL_BUILD_ERROR){
			// Get the build log
			std::string name     = device.getInfo<CL_DEVICE_NAME>();
			std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
			std::cerr << "Build log for " << name << ":" << std::endl
			        << buildlog << std::endl;
		}
	}
}
