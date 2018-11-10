// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include "World.h"
#include "Transparent.h"

Transparent::Transparent ()
: Reflective(), specular_btdf(new PerfectTransmitter)
{}

Transparent::~Transparent()
{
    delete specular_btdf;
}

Transparent::Transparent(const Transparent& t)
: Reflective(t)
{
    if(t.specular_btdf){
        specular_btdf = t.specular_btdf->clone();
    }
    else specular_btdf = NULL;
}

Transparent& Transparent::operator=(const Transparent& rhs){
	if (this == &rhs)
		return (*this);

    Reflective::operator=(rhs);

    if(specular_btdf){
        delete specular_btdf;
        specular_btdf = NULL;
    }
    if(rhs.specular_btdf){
        specular_btdf = rhs.specular_btdf->clone();
    }

	return (*this);
}

Transparent* Transparent::clone() const{
    return (new Transparent(*this));
}

RGBColor Transparent::shade(ShadeRec& sr) {
    RGBColor L(Phong::shade(sr));

    Vector3D wo = -sr.ray.d;
    Vector3D wi;
    RGBColor fr = reflective_brdf->sample_f(sr, wi, wo); // computes wi
    Ray reflected_ray(sr.hit_point, wi);

    if(specular_btdf->tir(sr)){
        L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);
        // kr = 1.0
    }
    else {
        Vector3D wt;
        RGBColor ft = specular_btdf->sample_f(sr, wt, wo); // computes wt
        Ray transmitted_ray(sr.hit_point, wt);

        L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
           * (sr.normal * wi);
        L += ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, sr.depth + 1);
    }

    return (L);
}

RGBColor Transparent::area_light_shade(ShadeRec& sr) {
    RGBColor L(Phong::shade(sr));

    Vector3D wo = -sr.ray.d;
    Vector3D wi;
    RGBColor fr = reflective_brdf->sample_f(sr, wi, wo); // computes wi
    Ray reflected_ray(sr.hit_point, wi);

    if(specular_btdf->tir(sr)){
        L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);
        // kr = 1.0
    }
    else {
        Vector3D wt;
        RGBColor ft = specular_btdf->sample_f(sr, wt, wo); // computes wt
        Ray transmitted_ray(sr.hit_point, wt);

        L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
           * (sr.normal * wi);
        L += ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, sr.depth + 1);
    }

    return (L);
}

CLMaterial Transparent::get_cl_material(){
    CLMaterial ret;
    ret.diffuse_brdf = reflective_brdf->get_cl_brdf();
    ret.specular_brdf = specular_btdf->get_cl_btdf();
    return ret;
}
