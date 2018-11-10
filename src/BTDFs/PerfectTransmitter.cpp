// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include <math.h>

#include "Constants.h"
#include "PerfectTransmitter.h"

PerfectTransmitter::PerfectTransmitter()
: BTDF(), kt(0.15), ior(1.5)
{}

PerfectTransmitter::~PerfectTransmitter()
{}

PerfectTransmitter::PerfectTransmitter(const PerfectTransmitter& pt)
:  BTDF(pt), kt(pt.kt), ior(pt.ior)
{}

PerfectTransmitter& PerfectTransmitter::operator=(const PerfectTransmitter& rhs){
	if (this == &rhs)
		return (*this);
	BTDF::operator=(rhs);

	kt = rhs.kt;
	ior = rhs.ior;

	return (*this);
}

PerfectTransmitter* PerfectTransmitter::clone() const {
    return new PerfectTransmitter(*this);
}

RGBColor PerfectTransmitter::sample_f(const ShadeRec& sr, Vector3D& wt, const Vector3D& wo) const {
    Normal n(sr.normal);
    float cos_thetai = n * wo;
    float eta = ior;

    if (cos_thetai < 0.0) {
        cos_thetai = -cos_thetai;
        n = -n;
        eta = 1.0 / eta;
    }

    float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
    float cos_theta2 = sqrt(temp);
    wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;

    return (kt / (eta * eta) * white / fabs(sr.normal * wt));
}

bool PerfectTransmitter::tir(const ShadeRec& sr) const {
    Vector3D wo(-sr.ray.d);
    float cos_thetai = sr.normal * wo;
    float eta = ior;

    if (cos_thetai < 0.0)
        eta = 1.0 / eta;

    return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta)) < 0.0;
}

CLBRDF PerfectTransmitter::get_cl_btdf() {
    CLBRDF ret;
    ret.kd = (cl_float) kt;
    ret.ks = (cl_float) ior;
    return ret;
}
