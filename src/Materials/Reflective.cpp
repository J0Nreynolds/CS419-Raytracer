// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include "World.h"
#include "Reflective.h"

Reflective::Reflective (void)
    : Phong(),
    reflective_brdf(new PerfectSpecular)
{}

Reflective::~Reflective()
{
    delete reflective_brdf;
}

Reflective::Reflective(const Reflective& r)
: Phong(r)
{
    if(r.reflective_brdf){
        reflective_brdf = r.reflective_brdf->clone();
    }
    else reflective_brdf = NULL;
}

Reflective& Reflective::operator=(const Reflective& rhs){
	if (this == &rhs)
		return (*this);

    Phong::operator=(rhs);

    if(reflective_brdf){
        delete reflective_brdf;
        reflective_brdf = NULL;
    }
    if(rhs.reflective_brdf){
        reflective_brdf = rhs.reflective_brdf->clone();
    }

	return (*this);
}

Reflective* Reflective::clone() const{
    return (new Reflective(*this));
}

RGBColor Reflective::shade(ShadeRec& sr) {
    RGBColor L(Phong::shade(sr)); // direct illumination

    Vector3D wo = -sr.ray.d;
    Vector3D wi;
    RGBColor fr = reflective_brdf->sample_f(sr, wi, wo);
    Ray reflected_ray(sr.hit_point, wi);

    L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);

    return (L);
}

RGBColor Reflective::area_light_shade(ShadeRec& sr) {
    RGBColor L(Phong::area_light_shade(sr)); // direct illumination

    Vector3D wo = -sr.ray.d;
    Vector3D wi;
    RGBColor fr = reflective_brdf->sample_f(sr, wi, wo);
    Ray reflected_ray(sr.hit_point, wi);

    L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);

    return (L);
}

CLMaterial Reflective::get_cl_material(){
    CLMaterial ret;
    ret.diffuse_brdf = reflective_brdf->get_cl_brdf();
    return ret;
}
