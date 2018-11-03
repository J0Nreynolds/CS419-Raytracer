#ifndef __CL_MESH_TRIANGLE__
#define __CL_MESH_TRIANGLE__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "CLMaterial.h"

typedef struct CLMeshTriangle{
    CLMaterial material;
    cl_double3 normal;
	cl_int mesh_idx;
	cl_int idx0;
	cl_int idx1;
	cl_int idx2;
    cl_char is_smooth;
} CLMeshTriangle;

#endif
