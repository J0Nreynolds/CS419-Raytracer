#ifndef __CL_LIGHT__
#define __CL_LIGHT__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLLight{
	cl_double3 pos;
	cl_double3 dir;
	cl_float3  color;
	cl_float   ls;
	cl_bool    shadows;
} CLLight;

#endif
