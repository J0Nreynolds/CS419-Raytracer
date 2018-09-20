// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Light
#include "Light.h"

Light::Light()
: d(Vector3D(0, 0, -1)), color(RGBColor(1, 1, 1))
{}

Light::Light(Vector3D dir)
: d(dir), color(RGBColor(1, 1, 1))
{}
