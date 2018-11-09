// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class PerfectSpecular

#include "PerfectSpecular.h"

PerfectSpecular::PerfectSpecular()
: BRDF(), kr(1.0), cr(1.0)
{}

PerfectSpecular::PerfectSpecular(float _kr)
:  BRDF(), kr(_kr), cr(1.0)
{}

PerfectSpecular::PerfectSpecular(float _kr, RGBColor _cr)
:  BRDF(), kr(_kr), cr(_cr)
{}

PerfectSpecular::~PerfectSpecular(){}

PerfectSpecular::PerfectSpecular(const PerfectSpecular& ps)
: BRDF(ps), kr(ps.kr), cr(ps.cr)
{}

PerfectSpecular& PerfectSpecular::operator=(const PerfectSpecular& rhs){
	if (this == &rhs)
		return (*this);

    BRDF::operator=(rhs);
    kr = rhs.kr;
    cr = rhs.cr;

	return (*this);
}

PerfectSpecular* PerfectSpecular::clone() const{
    return (new PerfectSpecular(*this));
}

RGBColor PerfectSpecular::sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const {
    float ndotwo = sr.normal * wo;
    wi = -wo + 2.0 * sr.normal * ndotwo;

    return (kr * cr / (sr.normal * wi));
}

CLBRDF PerfectSpecular::get_cl_brdf(){
    CLBRDF ret;
    ret.cd = (cl_float3){cr.r,cr.g,cr.b};
    ret.kd = (cl_float) -kr;
    ret.ks = (cl_float) 0;
    ret.exp = (cl_float) 0;
    return ret;
}
