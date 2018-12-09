// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class SV_Lambertian

#include "Constants.h"
#include "SV_Lambertian.h"

SV_Lambertian::SV_Lambertian()
: BRDF(), kd(1.0), cd(NULL)
{}

SV_Lambertian::SV_Lambertian(float _kd)
:  BRDF(), kd(_kd), cd(NULL)
{}

SV_Lambertian::SV_Lambertian(Texture* _cd)
:  BRDF(), kd(1.0), cd(_cd)
{}

SV_Lambertian::SV_Lambertian(float _kd, Texture* _cd)
:  BRDF(), kd(_kd), cd(_cd)
{}

SV_Lambertian::~SV_Lambertian(){}

SV_Lambertian::SV_Lambertian(const SV_Lambertian& l)
: BRDF(l), kd(l.kd), cd(l.cd)
{}

SV_Lambertian& SV_Lambertian::operator=(const SV_Lambertian& rhs){
	if (this == &rhs)
		return (*this);

    BRDF::operator=(rhs);
    kd = rhs.kd;
    if(rhs.cd){
        delete cd;
        cd = rhs.cd;
    }
    else cd = NULL;

	return (*this);
}

SV_Lambertian* SV_Lambertian::clone() const{
    return (new SV_Lambertian(*this));
}

RGBColor SV_Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const {
	 return (kd * cd->get_color(sr));
}

RGBColor SV_Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	return (kd * cd->get_color(sr) * invPI);
}

RGBColor SV_Lambertian::sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo, float& pdf) const {
    Vector3D w = sr.normal;
    Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w;
    u.normalize();
    Vector3D v = u ^ w;

    Point3D sp = sampler_ptr->sample_hemisphere();
    wi = sp.x * u + sp.y * v + sp.z * w;
    wi.normalize();

    pdf = sr.normal * wi * invPI;
	return (kd * cd->get_color(sr) * invPI);
}

CLBRDF SV_Lambertian::get_cl_brdf(){
    CLBRDF ret;
    ret.cd = (cl_float3){0, 0, 0};
    ret.kd = (cl_float) kd;
    ret.ks = (cl_float) 0;
    ret.exp = (cl_float) 0;
    return ret;
}
