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
#include "World.h"

class CLUtil {
	public:
        static cl::Device choose_platform_and_device();
		static CLSphere* get_cl_spheres(const World& world);
		static void attempt_build_program(cl::Program program, cl::Device device);
};

#endif
