// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class FBmTexture
#include "Constants.h"
#include "CubicNoise.h"
#include "FBmTexture.h"


FBmTexture::FBmTexture()
: noise_ptr(new CubicNoise()), color(black), min_value(0), max_value(1)
{}

FBmTexture::FBmTexture(const LatticeNoise* noise)
: noise_ptr(noise), color(black), min_value(0), max_value(1)
{}

FBmTexture::FBmTexture(const LatticeNoise* noise, const RGBColor& c)
: noise_ptr(noise), color(c), min_value(0), max_value(1)
{}

FBmTexture::FBmTexture(const LatticeNoise* noise, const RGBColor& c,
    float minval, float maxval)
: noise_ptr(noise), color(c), min_value(minval), max_value(maxval)
{}

FBmTexture::FBmTexture(const FBmTexture& fbmt)
: noise_ptr(fbmt.noise_ptr->clone()), color(fbmt.color), min_value(fbmt.min_value),
    max_value(fbmt.max_value)
{}

FBmTexture& FBmTexture::operator= (const FBmTexture& rhs)
{
    Texture::operator=(rhs);
    if(noise_ptr)
        delete noise_ptr;
    noise_ptr = rhs.noise_ptr->clone();
    color = rhs.color;
    min_value = rhs.min_value;
    max_value = rhs.max_value;
    return (*this);
}

FBmTexture::~FBmTexture()
{
    if(noise_ptr)
        delete noise_ptr;
}

FBmTexture* FBmTexture::clone() const
{
    return new FBmTexture(*this);
}

RGBColor FBmTexture::get_color(const ShadeRec& sr) const {
    float value = noise_ptr->value_fBm(sr.local_hit_point);
    value = min_value + (max_value - min_value) * value;

    return (value * color);
}
