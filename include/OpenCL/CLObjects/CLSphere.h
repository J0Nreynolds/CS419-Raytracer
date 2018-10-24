#ifndef __CL_SPHERE__
#define __CL_SPHERE__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION 
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "CLMaterial.h"

typedef struct CLSphere{
    CLMaterial material;
	cl_double3 center;
	cl_double radius;
} CLSphere;

#endif
