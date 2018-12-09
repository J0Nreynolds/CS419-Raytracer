// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include "Constants.h"
#include "BRDF.h"

BRDF::BRDF()
: sampler_ptr(NULL)
{}

BRDF::~BRDF(){
    if(sampler_ptr){
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
}

BRDF::BRDF(const BRDF& brdf){
    if(brdf.sampler_ptr){
        sampler_ptr = brdf.sampler_ptr->clone();
    }
    else sampler_ptr = NULL;
}

BRDF& BRDF::operator=(const BRDF& rhs){
	if (this == &rhs)
		return (*this);

    if(sampler_ptr){
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    if(rhs.sampler_ptr){
        sampler_ptr = rhs.sampler_ptr->clone();
    }

	return (*this);
}

RGBColor BRDF::f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const {
    return black;
}

RGBColor BRDF::sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo, float& pdf) const {
    return black;
}

RGBColor BRDF::sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const {
    return black;
}

RGBColor BRDF::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return black;
}
