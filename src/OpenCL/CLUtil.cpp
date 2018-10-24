// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class CLUtil
using namespace std;

#include <vector>
#include <iostream>

#ifdef __APPLE__
#include <cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"

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

// template <class To, class From>
// void CLUtil::get_all_cl(const World& world, To*& output, int& num_type) {
//     num_type = 0;
//     int num_objects = world.objects.size();
// 	for(int i = 0; i < num_objects; i ++){
//         From* obj = dynamic_cast<From*> (world.objects[i]);
//         if(obj != NULL){
//             num_type ++;
//         }
// 	}
//     output = new To[num_type];
//     int j = 0;
//     for(int i = 0; i < num_objects && j < num_type; i++){
//         From* obj = dynamic_cast<From*> (world.objects[i]);
//         if(obj != NULL){
//             output[j++] = obj->get_cl();
//         }
//     }
// }

void CLUtil::get_cl_spheres(const World& world, CLSphere*& spheres, int& num_spheres){
	num_spheres = 0;
    int num_objects = world.objects.size();
	for(int i = 0; i < num_objects; i ++){
        Sphere* sphere = dynamic_cast<Sphere*> (world.objects[i]);
        if(sphere != NULL){
            num_spheres ++;
        }
	}
    spheres = new CLSphere[num_spheres];
    int j = 0;
    for(int i = 0; i < num_objects && j < num_spheres; i++){
        Sphere* sphere = dynamic_cast<Sphere*> (world.objects[i]);
        if(sphere != NULL){
            spheres[j++] = sphere->get_cl_sphere();
        }
    }
}

void CLUtil::get_cl_triangles(const World& world, CLTriangle*& triangles, int& num_triangles){
	num_triangles = 0;
    int num_objects = world.objects.size();
	for(int i = 0; i < num_objects; i ++){
        Triangle* triangle = dynamic_cast<Triangle*> (world.objects[i]);
        if(triangle != NULL){
            num_triangles ++;
        }
	}
    triangles = new CLTriangle[num_triangles];
    int j = 0;
    for(int i = 0; i < num_objects && j < num_triangles; i++){
        Triangle* triangle = dynamic_cast<Triangle*> (world.objects[i]);
        if(triangle != NULL){
            triangles[j++] = triangle->get_cl_triangle();
        }
    }
}

void CLUtil::get_cl_planes(const World& world, CLPlane*& planes, int& num_planes){
	num_planes = 0;
    int num_objects = world.objects.size();
	for(int i = 0; i < num_objects; i ++){
        Plane* plane = dynamic_cast<Plane*> (world.objects[i]);
        if(plane != NULL){
            num_planes ++;
        }
	}
    planes = new CLPlane[num_planes];
    int j = 0;
    for(int i = 0; i < num_objects && j < num_planes; i++){
        Plane* plane = dynamic_cast<Plane*> (world.objects[i]);
        if(plane != NULL){
            planes[j++] = plane->get_cl_plane();
        }
    }
}

void CLUtil::get_cl_lights(const World& world, CLLight*& lights, int& num_lights){
    num_lights = world.lights.size();
    lights = new CLLight[num_lights];
	for(int i = 0; i < num_lights; i ++){
        lights[i] = world.lights[i]->get_cl_light();
	}

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
