#ifndef __CL_SAMPLER__
#define __CL_SAMPLER__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

typedef struct CLSampler{
    cl_int num_samples;
    cl_int num_sets;
    cl_int samples_index;
    cl_char sampler_type;
} CLSampler;

#endif
