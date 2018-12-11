#ifndef __FBM_TEXTURE__
#define __FBM_TEXTURE__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Texture.h"
#include "LatticeNoise.h"

class FBmTexture: public Texture {
     public:
        FBmTexture();
        FBmTexture(const LatticeNoise* noise);
        FBmTexture(const LatticeNoise* noise, const RGBColor& c);
        FBmTexture(const LatticeNoise* noise, const RGBColor& c, float minval, float maxval);
        FBmTexture(const FBmTexture& cc);
        FBmTexture& operator= (const FBmTexture& rhs);
        virtual ~FBmTexture();
        virtual FBmTexture* clone() const;

        virtual RGBColor get_color(const ShadeRec& sr) const;

     private:
        const LatticeNoise* noise_ptr;
        RGBColor color;
        float min_value, max_value;       // scaling factors
};

#endif
