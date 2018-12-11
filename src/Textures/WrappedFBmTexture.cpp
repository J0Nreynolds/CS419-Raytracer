// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class WrappedFBmTexture
#include <math.h>
#include "Constants.h"
#include "CubicNoise.h"
#include "WrappedFBmTexture.h"


WrappedFBmTexture::WrappedFBmTexture()
: noise_ptr(new CubicNoise()), color(black), expansion_number(5.0)
{}

WrappedFBmTexture::WrappedFBmTexture(const LatticeNoise* noise)
: noise_ptr(noise), color(black), expansion_number(5.0)
{}

WrappedFBmTexture::WrappedFBmTexture(const LatticeNoise* noise, const RGBColor& c)
: noise_ptr(noise), color(c), expansion_number(5.0)
{}

WrappedFBmTexture::WrappedFBmTexture(const LatticeNoise* noise, const RGBColor& c, float expval)
: noise_ptr(noise), color(c), expansion_number(expval)
{}

WrappedFBmTexture::WrappedFBmTexture(const WrappedFBmTexture& fbmt)
: noise_ptr(fbmt.noise_ptr->clone()), color(fbmt.color), expansion_number(fbmt.expansion_number)
{}

WrappedFBmTexture& WrappedFBmTexture::operator= (const WrappedFBmTexture& rhs)
{
    Texture::operator=(rhs);
    if(noise_ptr)
        delete noise_ptr;
    noise_ptr = rhs.noise_ptr->clone();
    color = rhs.color;
    expansion_number = rhs.expansion_number;
    return (*this);
}

WrappedFBmTexture::~WrappedFBmTexture()
{
    if(noise_ptr)
        delete noise_ptr;
}

WrappedFBmTexture* WrappedFBmTexture::clone() const
{
    return new WrappedFBmTexture(*this);
}

RGBColor WrappedFBmTexture::get_color(const ShadeRec& sr) const {
    float noise = expansion_number * noise_ptr->value_fBm(sr.local_hit_point);
    float value = noise - floor(noise);

    return (value * color);
}
