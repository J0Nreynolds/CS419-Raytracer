// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class LinearNoise
#include <math.h>
#include "LinearNoise.h"


LinearNoise::LinearNoise()
: LatticeNoise()
{}

LinearNoise::LinearNoise(const LinearNoise& ln)
: LatticeNoise(ln)
{
}

LinearNoise& LinearNoise::operator= (const LinearNoise& rhs)
{
    LatticeNoise::operator=(rhs);
    return (*this);
}

LinearNoise::~LinearNoise()
{}


LinearNoise* LinearNoise::clone() const {
    return new LinearNoise(*this);
}

template<class T> T lerp(const float f, const T& a, const T& b) {
     return (a + f * (b - a));
}

// constructors, etc.
float LinearNoise::value_noise(const Point3D& p) const {
    int ix, iy, iz;
    float fx, fy, fz;
    float d[2][2][2];
    float x0, x1, x2, x3, y0, y1, z0;

    ix = floor(p.x);
    fx = p.x - ix;

    iy = floor(p.y);
    fy = p.y - iy;

    iz = floor(p.z);
    fz = p.z - iz;

    for (int k = 0; k <= 1; k++)
        for (int j = 0; j <= 1; j++)
            for (int i = 0; i <= 1; i++)
                d[k][j][i] = value_table[INDEX(ix + i, iy + j, iz + k)];

    x0 = lerp(fx, d[0][0][0], d[0][0][1]);
    x1 = lerp(fx, d[0][1][0], d[0][1][1]);
    x2 = lerp(fx, d[1][0][0], d[1][0][1]);
    x3 = lerp(fx, d[1][1][0], d[1][1][1]);
    y0 = lerp(fy, x0, x1);
    y1 = lerp(fy, x2, x3);
    z0 = lerp(fz, y0, y1);

    return (z0);
}
