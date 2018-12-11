// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Checker3D
#include <math.h>
#include "Constants.h"
#include "Checker3D.h"


Checker3D::Checker3D()
: color1(black), color2(white), size(1.0)
{}

Checker3D::Checker3D(const RGBColor& c1, const RGBColor& c2)
: color1(c1), color2(c2), size(1.0)
{}


Checker3D::Checker3D(const RGBColor& c1, const RGBColor& c2, float s)
: color1(c1), color2(c2), size(s)
{}

Checker3D::Checker3D(const Checker3D& cc)
: color1(cc.color1), color2(cc.color2), size(cc.size)
{}

Checker3D& Checker3D::operator= (const Checker3D& rhs)
{
    Texture::operator=(rhs);
    color1 = rhs.color1;
    color2 = rhs.color2;
    size = rhs.size;
    return (*this);
}

Checker3D::~Checker3D()
{}

Checker3D* Checker3D::clone() const
{
    return new Checker3D(*this);
}


RGBColor Checker3D::get_color(const ShadeRec& sr) const {

     float eps = -0.000187453738; // small random number
     float x = sr.local_hit_point.x + eps;
     float y = sr.local_hit_point.y + eps;
     float z = sr.local_hit_point.z + eps;

     if (((int)floor(x / size) + (int)floor(y / size) + (int)floor(z /
           size)) % 2 == 0) return (color1);
     else
          return (color2);
}
