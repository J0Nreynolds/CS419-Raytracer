#ifndef __CL_TRIANGLE__
#define __CL_TRIANGLE__

#ifdef __APPLE__
#include <OpenCL/cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLTriangle{
	cl_double3 p1;
	cl_double3 p2;
	cl_double3 p3;
} CLTriangle;

#endif
