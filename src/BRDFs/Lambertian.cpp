// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Lambertian

#include "Constants.h"
#include "Lambertian.h"

Lambertian::Lambertian()
: BRDF(), kd(1.0), cd(1.0)
{}

Lambertian::Lambertian(float _kd)
:  BRDF(), kd(_kd), cd(1.0)
{}

Lambertian::Lambertian(RGBColor _cd)
:  BRDF(), kd(1.0), cd(_cd)
{}

Lambertian::Lambertian(float _kd, RGBColor _cd)
:  BRDF(), kd(_kd), cd(_cd)
{}

Lambertian::~Lambertian(){}

Lambertian::Lambertian(const Lambertian& l)
: BRDF(l), kd(l.kd), cd(l.cd)
{}

Lambertian& Lambertian::operator=(const Lambertian& rhs){
	if (this == &rhs)
		return (*this);

    BRDF::operator=(rhs);
    kd = rhs.kd;
    cd = rhs.cd;

	return (*this);
}

Lambertian* Lambertian::clone() const{
    return (new Lambertian(*this));
}

RGBColor Lambertian::f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const {
    return (kd * cd * invPI);
}

RGBColor Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (kd * cd);
}

CLBRDF Lambertian::get_cl_brdf(){
    CLBRDF ret;
    ret.cd = (cl_float3){cd.r, cd.g, cd.b};
    ret.kd = (cl_float) kd;
    ret.ks = (cl_float) 0;
    ret.exp = (cl_float) 0;
    return ret;
}
