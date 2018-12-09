// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class SphericalMap
#include <math.h>
#include "Constants.h"
#include "SphericalMap.h"

SphericalMap::SphericalMap()
{}

SphericalMap::~SphericalMap()
{}

SphericalMap::SphericalMap(const SphericalMap& m)
{}

SphericalMap& SphericalMap::operator=(const SphericalMap& rhs)
{
    return (*this);
}


SphericalMap* SphericalMap::clone() const {
    return new SphericalMap(*this);
}

void SphericalMap::get_texel_coordinates(const Point3D& local_hit_point,
    const int hres, const int vres, int& row, int& column) const {
    // first, compute theta and phi
    float len = local_hit_point.distance(Point3D(0)); // TODO: revisit why/if this is needed
    float theta = acos(local_hit_point.y);
    float phi = atan2(local_hit_point.x, local_hit_point.z);
    if (phi < 0.0)
        phi += TWO_PI;

    // next, map theta and phi to (u, v) in [0, 1] X [0, 1]

    float u = phi * invTWO_PI;
    float v = 1 - theta * invPI;

    // finally, map u and v to the texel coordinates

    column = (int) ((hres - 1) * u);             // column is across
    row = (int)  ((vres - 1) * v);               // row is up
}
