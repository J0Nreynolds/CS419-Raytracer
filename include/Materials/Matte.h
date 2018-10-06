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

		virtual CLMaterial get_cl_material();

    private:

        Lambertian*    ambient_brdf;
        Lambertian*    diffuse_brdf;
};

inline void Matte::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}

inline void Matte::set_kd (const float kd) {
    diffuse_brdf->set_kd(kd);
}

inline void Matte::set_cd(const RGBColor& c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
}

#endif
