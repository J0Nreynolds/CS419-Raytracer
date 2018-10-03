// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Light
#include "Light.h"

Light::Light()
: shadows(true)
{}

Light::~Light(){}


CLLight Light::get_cl_light()
{
	CLLight ret;
	ret.shadows = (cl_bool)shadows;
	return ret;
}
