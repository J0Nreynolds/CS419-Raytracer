// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class LatticeNoise
#include <cstdlib>
#include <math.h>
#include "LatticeNoise.h"

// Using the 512 values that were used in Perlin's original implementation
const unsigned char LatticeNoise::permutation_table[] = {
  151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
  140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
  247,120,234, 75,  0, 26,197, 62 ,94,252,219,203,117, 35, 11, 32,
   57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
   74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
   60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
   65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
  200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
   52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
  207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
  119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
  129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
  218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
   81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
  184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
  222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180
};

LatticeNoise::LatticeNoise()
: fs_min(0.0), fs_max(1.0), fBm_min(-1.0), fBm_max(1.0)
{
    init_value_table();
}

LatticeNoise::LatticeNoise(const LatticeNoise& ln)
: fs_min(ln.fs_min), fs_max(ln.fs_max), fBm_min(ln.fBm_min), fBm_max(ln.fBm_max)
{
    for(int i = 0; i < kTableSize; i++){
        value_table[i] = ln.value_table[i];
    }
}

LatticeNoise& LatticeNoise::operator= (const LatticeNoise& rhs)
{
    for(int i = 0; i < kTableSize; i++){
        value_table[i] = rhs.value_table[i];
    }
    return (*this);
}

LatticeNoise::~LatticeNoise()
{}

void LatticeNoise::init_value_table() {
     for (int i = 0; i < kTableSize; i++){
         float random = (float) rand() / RAND_MAX;
         // in the range [-1, +1]
         value_table[i] = 1.0 - 2.0 * random;
     }
}

float LatticeNoise::value_fractal_sum(const Point3D& p) const {
    float amplitude = 1.0;
    float frequency = 1.0;
    float fractal_sum = 0.0;

    for (int j = 0; j < num_octaves; j++) {
        fractal_sum += amplitude * value_noise(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    fractal_sum = (fractal_sum - fs_min) / (fs_max - fs_min);

    return (fractal_sum);
}

float LatticeNoise::value_turbulence(const Point3D& p) const {
        float amplitude = 1.0;
        float frequency = 1.0;
        float turbulence = 0.0;

        for (int j = 0 ; j < num_octaves; j++) {
            turbulence += amplitude * fabs(value_noise(p * frequency));
            amplitude *= 0.5;
            frequency *= 2.0;
        }

        turbulence /= fs_max;

        return (turbulence);
}

float LatticeNoise::value_fBm(const Point3D& p) const {
     float amplitude = 1.0;
     float frequency = 1.0;
     float fBm = 0.0;

     for (int j = 0; j < num_octaves; j++) {
        fBm += amplitude * value_noise(p * frequency);
        amplitude *= gain;
        frequency *= lacunarity;
     }

     fBm = (fBm - fBm_min) / (fBm_max - fBm_min);

     return (fBm);
}
