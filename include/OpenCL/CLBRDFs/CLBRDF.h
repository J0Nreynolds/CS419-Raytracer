#ifndef __CL_BRDF__
#define __CL_BRDF__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION 
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLBRDF{
    cl_float3 cd;
    cl_float kd;
    cl_float ks;
    cl_float exp;
} CLBRDF;

#endif
