// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Emissive
#include "World.h"
#include "Emissive.h"

#include <iostream>

Emissive::Emissive()
    : Material(), ls(1.0), ce(1.0)
{}

Emissive::~Emissive()
{}

Emissive::Emissive(const Emissive& e)
: Material(e), ls(e.ls), ce(e.ce)
{}

Emissive& Emissive::operator=(const Emissive& rhs){
	if (this == &rhs)
		return (*this);

    Material::operator=(rhs);
    ls = rhs.ls;
    ce = rhs.ce;

	return (*this);
}

Emissive* Emissive::clone() const{
    return (new Emissive(*this));
}

RGBColor Emissive::shade(ShadeRec& sr) {
     if (-sr.normal * sr.ray.d > 0.0)
          return (ls * ce);
     else
          return (black);
}

RGBColor Emissive::area_light_shade(ShadeRec& sr) {
     if (-sr.normal * sr.ray.d > 0.0)
          return (ls * ce);
     else
          return (black);
}

CLMaterial Emissive::get_cl_material(){
    CLMaterial ret;
    ret.specular_brdf.cd = (cl_float3){0, 0, 0};
    ret.specular_brdf.kd = 0;
    ret.specular_brdf.exp = 0;
    ret.specular_brdf.ks = 0;
    return ret;
}
