// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class GlossySpecular

#include <math.h>
#include "Constants.h"
#include "GlossySpecular.h"

GlossySpecular::GlossySpecular()
: BRDF(), ks(1.0), exp(1.0)
{}

GlossySpecular::GlossySpecular(float _ks)
:  BRDF(), ks(_ks), exp(1.0)
{}
GlossySpecular::GlossySpecular(float _ks, float _exp)
:  BRDF(), ks(_ks), exp(_exp)
{}

GlossySpecular::~GlossySpecular(){}

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

RGBColor GlossySpecular::sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const {
    return black;
}

RGBColor GlossySpecular::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return black;
}

CLBRDF GlossySpecular::get_cl_brdf(){
    CLBRDF ret;
    ret.cd = (cl_float3){0,0,0};
    ret.kd = (cl_float) 0;
    ret.ks = (cl_float) ks;
    ret.exp = (cl_float) exp;
    return ret;
}