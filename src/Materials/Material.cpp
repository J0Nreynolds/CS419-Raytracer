// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Material
#include "Constants.h"
#include "Material.h"

Material::Material()
{}

RGBColor Material::shade(ShadeRec& sr){
    return black;
}

RGBColor Material::area_light_shade(ShadeRec& sr){
    return black;
}

RGBColor Material::path_shade(ShadeRec& sr){
    return black;
}
