// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class GlossySpecular

#include <math.h>
#include "Constants.h"
#include "MultiJittered.h"
#include "GlossySpecular.h"

GlossySpecular::GlossySpecular()
: BRDF(), ks(1.0), exp(1.0), sampler_ptr(NULL)
{}

GlossySpecular::GlossySpecular(float _ks)
:  BRDF(), ks(_ks), exp(1.0), sampler_ptr(NULL)
{}
GlossySpecular::GlossySpecular(float _ks, float _exp)
:  BRDF(), ks(_ks), exp(_exp), sampler_ptr(NULL)
{}

GlossySpecular::~GlossySpecular(){}

GlossySpecular::GlossySpecular(const GlossySpecular& gs)
: BRDF(gs), ks(gs.ks), exp(gs.exp)
{
	if(gs.sampler_ptr){
	    sampler_ptr = gs.sampler_ptr->clone();
	}
	else sampler_ptr = NULL;
}

GlossySpecular& GlossySpecular::operator=(const GlossySpecular& rhs){
	if (this == &rhs)
		return (*this);

    BRDF::operator=(rhs);
    ks = rhs.ks;
    exp = rhs.exp;

	if(rhs.sampler_ptr){
	    sampler_ptr = rhs.sampler_ptr->clone();
	}
	else sampler_ptr = NULL;

	return (*this);
}

GlossySpecular* GlossySpecular::clone() const{
    return (new GlossySpecular(*this));
}

RGBColor GlossySpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D&
                    wi) const {
    RGBColor L;
    float ndotwi = sr.normal * wi;
    Vector3D r(-wi + 2.0 * sr.normal * ndotwi);
    float rdotwo = r * wo;

    if (rdotwo > 0.0)
        L = ks * pow(rdotwo, exp);

    return (L);
}

void GlossySpecular::set_samples(const int num_samples, const float exp) {
    sampler_ptr = new MultiJittered(num_samples);
    sampler_ptr->map_samples_to_hemisphere(exp);
}

void GlossySpecular::set_sampler(Sampler* sampler) {
    sampler_ptr = sampler;
    sampler_ptr->map_samples_to_hemisphere(exp);
}

RGBColor GlossySpecular::sample_f(const ShadeRec& sr, Vector3D& wi,
    const Vector3D& wo, float& pdf) const {
    float ndotwo = sr.normal * wo;
    Vector3D r = -wo + 2.0 * sr.normal * ndotwo; // direction of mirror
                                                 // reflection
    Vector3D w = r;
    Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w;
    u.normalize();
    Vector3D v = u ^ w;

    Point3D sp = sampler_ptr->sample_hemisphere();
    wi = sp.x * u + sp.y * v + sp.z * w; // reflected ray direction

    if (sr.normal * wi < 0.0) // reflected ray is below surface
         wi = -sp.x * u - sp.y * v + sp.z * w;

    float phong_lobe = pow(r * wi, exp);
    pdf = phong_lobe * (sr.normal * wi);

    return (ks * cs * phong_lobe);
}

CLBRDF GlossySpecular::get_cl_brdf(){
    CLBRDF ret;
    ret.cd = (cl_float3){0,0,0};
    ret.kd = (cl_float) 0;
    ret.ks = (cl_float) ks;
    ret.exp = (cl_float) exp;
    return ret;
}
