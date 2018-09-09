#ifndef __CL_SPHERE__
#define __CL_SPHERE__

#ifdef __APPLE__
#include <OpenCL/cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLSphere{
	cl_double3 center;
	cl_float3 color;
	cl_double radius;
} CLSphere;

#endif
