// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Ambient
#include "Ambient.h"

Ambient::Ambient()
: Light(), color(1.0), ls(1.0)
{
	set_shadows(false);
}

Ambient::Ambient(RGBColor c)
: Light(), color(c), ls(1.0)
{
	set_shadows(false);
}

Ambient::~Ambient()
{}

Ambient::Ambient(const Ambient& a)
: Light(a), color(a.color), ls(a.ls)
{
}

Ambient& Ambient::operator=(const Ambient& rhs){
	if (this == &rhs)
		return (*this);

	Light::operator=(rhs);

	color = rhs.color;
	ls = rhs.ls;

	return (*this);
}

Ambient* Ambient::clone() const{
	return (new Ambient(*this));
}

RGBColor Ambient::L(ShadeRec& sr)
{
    return ls * color;
}

Vector3D Ambient::get_direction(ShadeRec& sr)
{
    return Vector3D(0, 0, 0);
}

bool Ambient::in_shadow(const Ray& ray, const ShadeRec& sr) const {
	return false;
}

CLLight Ambient::get_cl_light()
{
	CLLight ret = Light::get_cl_light();
	ret.dir = (cl_double3){0, 0, 0};
	ret.pos = (cl_double3){0, 0, 0};
    ret.color = (cl_float3){color.r, color.g, color.b};
    ret.ls = (cl_float)ls;
	return ret;
}
