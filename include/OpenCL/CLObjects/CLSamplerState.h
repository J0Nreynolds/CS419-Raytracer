#ifndef __CL_SAMPLER_STATE__
#define __CL_SAMPLER_STATE__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLSamplerState{
    cl_ulong seed;
    cl_int jump;
    cl_int count;
} CLSamplerState;

#endif
