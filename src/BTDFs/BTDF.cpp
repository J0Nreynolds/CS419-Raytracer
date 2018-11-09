// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include "Constants.h"
#include "BTDF.h"

BTDF::BTDF()
{}

BTDF::~BTDF()
{}

BTDF::BTDF(const BTDF& btdf){

}

BTDF& BTDF::operator=(const BTDF& rhs){
	if (this == &rhs)
		return (*this);

	return (*this);
}

RGBColor BTDF::f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const {
    return black;
}

RGBColor BTDF::sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const{
    return black;
}

RGBColor BTDF::rho(const ShadeRec& sr, const Vector3D& wo) const{
    return black;
}
