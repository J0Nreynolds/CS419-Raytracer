// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class CubicNoise
#include <algorithm>
#include <math.h>
#include "CubicNoise.h"


CubicNoise::CubicNoise()
: LatticeNoise()
{}

CubicNoise::CubicNoise(const CubicNoise& cn)
: LatticeNoise(cn)
{
}

CubicNoise& CubicNoise::operator= (const CubicNoise& rhs)
{
    LatticeNoise::operator=(rhs);
    return (*this);
}

CubicNoise::~CubicNoise()
{}


CubicNoise* CubicNoise::clone() const {
    return new CubicNoise(*this);
}

template<class T> T four_knot_spline(const float x, const T knots[]) {
     T c3 = -0.5 * knots[0] + 1.5 * knots[1] - 1.5 * knots[2] + 0.5 * knots[3];
     T c2 = knots[0] - 2.5 * knots[1] + 2.0 * knots[2] - 0.5 * knots[3];
     T c1 = 0.5 * (-knots[0] + knots [2]);
     T c0 = knots[1];

     return (T((c3*x + c2)*x + c1)*x + c0);
}

// constructors, etc.
float CubicNoise::value_noise(const Point3D& p) const {
    int ix, iy, iz;
    float fx, fy, fz;
    float xknots[4], yknots[4], zknots[4];

    ix = floor(p.x);
    fx = p.x - ix;

    iy = floor(p.y);
    fy = p.y - iy;

    iz = floor(p.z);
    fz = p.z - iz;

    for (int k = -1; k <= 2; k++) {
        for (int j = -1; j <= 2; j++) {
            for (int i = -1; i <= 2; i++) {
                xknots[i+1] = value_table[INDEX(ix + i, iy + j, iz + k)];
            }
            yknots[j+1] = four_knot_spline(fx, xknots);
        }
        zknots[k+1] = four_knot_spline(fy, yknots);
    }

    return (std::clamp(four_knot_spline(fz, zknots), (float) -1.0, (float) 1.0));
}
