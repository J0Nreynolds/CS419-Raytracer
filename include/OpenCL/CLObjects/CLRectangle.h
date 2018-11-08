#ifndef __CL_RECTANGLE__
#define __CL_RECTANGLE__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "CLMaterial.h"

typedef struct CLRectangle{
	CLMaterial material;
	cl_double3 p0;
	cl_double3 a;
	cl_double3 b;
	cl_double3 normal;
	cl_double a_len_squared;	// square of the length of side a
	cl_double b_len_squared;	// square of the length of side b
	cl_float area;			// for rectangular lights
	cl_float inv_area;		// for rectangular lights
	cl_int sampler_index;	// for rectangular lights
} CLRectangle;

#endif
