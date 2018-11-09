#ifndef __CL_MATERIAL__
#define __CL_MATERIAL__

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "CLBRDF.h"

typedef struct CLMaterial{
    CLBRDF ambient_brdf;
    CLBRDF diffuse_brdf;
    CLBRDF specular_brdf;
} CLMaterial;

#endif
