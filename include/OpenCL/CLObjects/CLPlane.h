#ifndef __CL_PLANE__
#define __CL_PLANE__

#ifdef __APPLE__
#include <OpenCL/cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLPlane{
	cl_double3 a;
	cl_double3 n;
	cl_float3 color;
} CLPlane;

#endif
