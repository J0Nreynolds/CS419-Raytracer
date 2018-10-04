// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Ambient
#include "Ambient.h"

Ambient::Ambient()
: Light(), color(1.0), ls(1.0)
{}

Ambient::Ambient(RGBColor c)
: Light(), color(c), ls(1.0)
{}

Ambient::~Ambient()
{}

CLLight Ambient::get_cl_light()
{
	CLLight ret = Light::get_cl_light();
    ret.color = (cl_float3){color.r, color.g, color.b};
    ret.ls = (cl_float)ls;
	return ret;
}

RGBColor Ambient::L(ShadeRec sr)
{
    return ls * color;
}

Vector3D Ambient::get_direction(ShadeRec sr)
{
    return Vector3D(0, 0, 0);
}