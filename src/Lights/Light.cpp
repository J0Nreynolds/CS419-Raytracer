// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Light
#include "Light.h"

Light::Light()
: shadows(true)
{}

Light::~Light()
{}

Light::Light(const Light& ls)
:shadows(ls.shadows)
{
}

Light& Light::operator= (const Light& rhs){
	shadows = rhs.shadows;
}

float Light::G(const ShadeRec& sr) const {
	return 1.0;
}

float Light::pdf(const ShadeRec& sr) const {
	return 1.0;
}

CLLight Light::get_cl_light()
{
	CLLight ret;
	ret.shadows = (cl_char)shadows;
	return ret;
}
