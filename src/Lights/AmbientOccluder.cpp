// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class AmbientOccluder
#include "AmbientOccluder.h"

AmbientOccluder::AmbientOccluder()
: Ambient(), sampler_ptr(NULL), min_amount(0.5)
{
}

AmbientOccluder::AmbientOccluder(RGBColor c)
: Ambient(c), sampler_ptr(NULL), min_amount(0.5)
{
}

AmbientOccluder::AmbientOccluder(RGBColor c, float amount)
: Ambient(c), sampler_ptr(NULL), min_amount(amount)
{
}

AmbientOccluder::~AmbientOccluder()
{
    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
}

AmbientOccluder::AmbientOccluder(const AmbientOccluder& ao)
: Ambient(ao), u(ao.u), v(ao.v), w(ao.w), min_amount(ao.min_amount)
{
    if(sampler_ptr){
    	delete sampler_ptr;
    	sampler_ptr = NULL;
    }
    if(ao.sampler_ptr){
    	sampler_ptr = ao.sampler_ptr->clone();
    }
}

AmbientOccluder& AmbientOccluder::operator=(const AmbientOccluder& rhs){
	if (this == &rhs)
		return (*this);

	Light::operator=(rhs);

    u = rhs.u;
    v = rhs.v;
    w = rhs.w;
    min_amount = rhs.min_amount;

    if(sampler_ptr){
    	delete sampler_ptr;
    	sampler_ptr = NULL;
    }
    if(rhs.sampler_ptr){
    	sampler_ptr = rhs.sampler_ptr->clone();
    }

	return (*this);
}

AmbientOccluder* AmbientOccluder::clone() const{
	return (new AmbientOccluder(*this));
}

RGBColor AmbientOccluder::L(ShadeRec& sr)
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

Vector3D AmbientOccluder::get_direction(ShadeRec& sr)
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

CLLight AmbientOccluder::get_cl_light()
{
	CLLight ret = Ambient::get_cl_light();
	ret.dir = (cl_double3){min_amount.r, min_amount.g, min_amount.b};
	ret.pos = (cl_double3){0, 0, 0};
    ret.color = (cl_float3){color.r, color.g, color.b};
    ret.ls = (cl_float)ls;
    ret.shadows = (cl_bool) true;
    ret.sampler_index = sampler_ptr->get_cl_index();
	return ret;
}
