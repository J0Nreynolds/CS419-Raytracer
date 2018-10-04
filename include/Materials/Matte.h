#ifndef __MATTE__
#define __MATTE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "RGBColor.h"
#include "Lambertian.h"
#include "Material.h"

class Matte: public Material {

    public:

        Matte();

        void set_ka(const float k);

        void set_kd(const float k);

        void set_cd(const RGBColor& c);

        virtual RGBColor shade(ShadeRec& sr);

    private:

        Lambertian*    ambient_brdf;
        Lambertian*    diffuse_brdf;
};

#endif
