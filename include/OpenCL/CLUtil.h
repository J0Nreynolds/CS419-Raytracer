#ifndef __CL_UTIL__
#define __CL_UTIL__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

#ifdef __APPLE__
#include <OpenCL/cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "CLSphere.h"
#include "CLTriangle.h"
#include "CLPlane.h"
#include "World.h"

class CLUtil {
	public:
        static cl::Device choose_platform_and_device();
		// template <> static void get_all_cl<CLSphere, Sphere>(const World& world, CLSphere*& output, int& num_type);
		static void get_cl_spheres(const World& world, CLSphere*& spheres, int& num_spheres);
		static void get_cl_triangles(const World& world, CLTriangle*& triangles, int& num_triangles);
		static void get_cl_planes(const World& world, CLPlane*& planes, int& num_planes);
		static void get_cl_lights(const World& world, CLLight*& lights, int& num_lights);
		static void attempt_build_program(cl::Program program, cl::Device device);
};

#endif
