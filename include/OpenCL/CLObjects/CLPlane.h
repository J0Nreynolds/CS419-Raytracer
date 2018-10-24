#ifndef __CL_PLANE__
#define __CL_PLANE__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION 
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "CLMaterial.h"

typedef struct CLPlane{
    CLMaterial material;
	cl_double3 a;
	cl_double3 n;
} CLPlane;

#endif
