#ifndef __CL_TRIANGLE__
#define __CL_TRIANGLE__

#ifdef __APPLE__
#include <OpenCL/cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLTriangle{
	cl_double3 v0;
	cl_double3 v1;
	cl_double3 v2;
} CLTriangle;

#endif
