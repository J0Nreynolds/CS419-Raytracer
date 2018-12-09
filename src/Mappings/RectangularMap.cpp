// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class RectangularMap
#include <math.h>
#include "Constants.h"
#include "RectangularMap.h"

RectangularMap::RectangularMap()
{}

RectangularMap::~RectangularMap()
{}

RectangularMap::RectangularMap(const RectangularMap& m)
{}

RectangularMap& RectangularMap::operator=(const RectangularMap& rhs)
{
    return (*this);
}


RectangularMap* RectangularMap::clone() const {
    return new RectangularMap(*this);
}

void RectangularMap::get_texel_coordinates(const Point3D& local_hit_point,
    const int hres, const int vres, int& row, int& column) const {
    float u = (local_hit_point.z + 1.0 )/ 2.0;
    float v = (local_hit_point.x + 1.0 )/ 2.0;
    column = (int) ((hres - 1) * u);             // column is across
    row = (int)  ((vres - 1) * v);               // row is up
}
