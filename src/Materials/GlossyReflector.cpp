// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include "World.h"
#include "GlossyReflector.h"

GlossyReflector::GlossyReflector (void)
    : Phong(),
    glossy_specular_brdf(new GlossySpecular)
{}

GlossyReflector::~GlossyReflector()
{
    delete glossy_specular_brdf;
}

GlossyReflector::GlossyReflector(const GlossyReflector& gr)
: Phong(gr)
{
    if(gr.glossy_specular_brdf){
        glossy_specular_brdf = gr.glossy_specular_brdf->clone();
    }
    else glossy_specular_brdf = NULL;
}

GlossyReflector& GlossyReflector::operator=(const GlossyReflector& rhs){
	if (this == &rhs)
		return (*this);

    Phong::operator=(rhs);

    if(glossy_specular_brdf){
        delete glossy_specular_brdf;
        glossy_specular_brdf = NULL;
    }
    if(rhs.glossy_specular_brdf){
        glossy_specular_brdf = rhs.glossy_specular_brdf->clone();
    }

	return (*this);
}

GlossyReflector* GlossyReflector::clone() const{
    return (new GlossyReflector(*this));
}

RGBColor GlossyReflector::area_light_shade(ShadeRec& sr) {
    RGBColor L(Phong::area_light_shade(sr)); // direct illumination
    Vector3D wo(-sr.ray.d);
    Vector3D wi;
    float pdf;
    RGBColor fr(glossy_specular_brdf->sample_f(sr, wi, wo, pdf));
    Ray reflected_ray(sr.hit_point, wi);

    L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf;

    return (L);
}

CLMaterial GlossyReflector::get_cl_material(){
    CLMaterial ret = Phong::get_cl_material();
    return ret;
}
