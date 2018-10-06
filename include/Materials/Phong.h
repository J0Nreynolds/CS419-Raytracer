#ifndef __PHONG__
#define __PHONG__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "RGBColor.h"
#include "Lambertian.h"
#include "GlossySpecular.h"
#include "Material.h"

class Phong: public Material {

    public:

        Phong();

        void set_ka(const float k);

        void set_kd(const float k);

        void set_ks(const float k);

        void set_exp(const float e);

        void set_cd(const RGBColor& c);

        virtual RGBColor shade(ShadeRec& sr);

		virtual CLMaterial get_cl_material();

    private:

        Lambertian*      ambient_brdf;
        Lambertian*      diffuse_brdf;
        GlossySpecular*  specular_brdf;
};

inline void Phong::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}

inline void Phong::set_kd(const float kd) {
    diffuse_brdf->set_kd(kd);
}

inline void Phong::set_ks(const float ks) {
    specular_brdf->set_ks(ks);
}

inline void Phong::set_exp(const float e) {
    specular_brdf->set_exp(e);
}

inline void Phong::set_cd(const RGBColor& c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
}

#endif
