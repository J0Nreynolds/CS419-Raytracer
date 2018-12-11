#ifndef __WRAPPED_FBM_TEXTURE__
#define __WRAPPED_FBM_TEXTURE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

#include "Texture.h"
#include "LatticeNoise.h"

class WrappedFBmTexture: public Texture {
        public:
            WrappedFBmTexture();
            WrappedFBmTexture(const LatticeNoise* noise);
            WrappedFBmTexture(const LatticeNoise* noise, const RGBColor& c);
            WrappedFBmTexture(const LatticeNoise* noise, const RGBColor& c, float expval);
            WrappedFBmTexture(const WrappedFBmTexture& cc);
            WrappedFBmTexture& operator= (const WrappedFBmTexture& rhs);
            virtual ~WrappedFBmTexture();
            virtual WrappedFBmTexture* clone() const;

            virtual RGBColor get_color(const ShadeRec& sr) const;
        private:
            const LatticeNoise* noise_ptr;
            RGBColor color;
            float expansion_number;
};

#endif
