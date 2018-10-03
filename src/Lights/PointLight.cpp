// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class PointLight
#include "PointLight.h"

PointLight::PointLight()
: Light(), p(0), color(1.0), ls(1.0)
{}

PointLight::PointLight(Point3D pos)
: Light(), p(pos), color(1.0), ls(1.0)
{}

PointLight::PointLight(RGBColor c)
: Light(), p(0), color(c), ls(1.0)
{}

PointLight::PointLight(RGBColor c, Point3D pos)
: Light(), p(pos), color(c), ls(1.0)
{}

PointLight::~PointLight()
{}

CLLight PointLight::get_cl_light()
{
	CLLight ret = Light::get_cl_light();
	ret.pos = (cl_double3){p.x, p.y, p.z};
    ret.color = (cl_float3){color.r, color.g, color.b};
    ret.ls = (cl_float)ls;
	return ret;
}

RGBColor PointLight::L(ShadeRec sr)
{
    return ls * color;
}

Vector3D PointLight::get_direction(ShadeRec sr)
{
    return((p - sr.local_hit_point).hat());
}
