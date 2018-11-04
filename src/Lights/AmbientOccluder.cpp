// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class AmbientOccluder
#include "AmbientOccluder.h"

AmbientOccluder::AmbientOccluder()
: Ambient(), min_amount(0.5), sampler_ptr(NULL)
{
}

AmbientOccluder::AmbientOccluder(RGBColor c)
: Ambient(c), min_amount(0.5), sampler_ptr(NULL)
{
}

AmbientOccluder::AmbientOccluder(RGBColor c, float amount)
: Ambient(c), min_amount(amount), sampler_ptr(NULL)
{
}

AmbientOccluder::~AmbientOccluder()
{
    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
}

CLLight AmbientOccluder::get_cl_light()
{
	CLLight ret = Light::get_cl_light();
	ret.dir = (cl_double3){min_amount.r, min_amount.g, min_amount.b};
	ret.pos = (cl_double3){0, 0, 0};
    ret.color = (cl_float3){color.r, color.g, color.b};
    ret.ls = (cl_float)ls;
    ret.shadows = (cl_char)true;
	return ret;
}

RGBColor AmbientOccluder::L(ShadeRec sr)
{
    w = sr.normal;
    // jitter up vector in case normal is vertical
    v = w ^ Vector3D(0.0072, 1.0, 0.0034);
    v.normalize();
    u = v ^ w;

    Ray shadow_ray;
    shadow_ray.o = sr.hit_point;
    shadow_ray.d = get_direction(sr);

    if (in_shadow(shadow_ray, sr))
        return (min_amount * ls * color);
    else
        return (ls * color);
}

Vector3D AmbientOccluder::get_direction(ShadeRec sr)
{
    Point3D sp = sampler_ptr->sample_hemisphere();
    return (sp.x * u + sp.y * v + sp.z * w);
}

bool AmbientOccluder::in_shadow(const Ray& ray, const ShadeRec& sr) const {
    float t;
    int num_objects = sr.w.objects.size();
    for (int j = 0; j < num_objects; j++){
        if (sr.w.objects[j]->shadow_hit(ray, t)){
            return true;
        }
    }
    return false;
}
