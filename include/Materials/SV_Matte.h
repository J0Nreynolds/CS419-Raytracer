#ifndef __SV_MATTE__
#define __SV_MATTE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "RGBColor.h"
#include "SV_Lambertian.h"
#include "Material.h"

class SV_Matte: public Material {

    public:

        SV_Matte();
        virtual ~SV_Matte();
		SV_Matte(const SV_Matte& e);
        SV_Matte& operator=(const SV_Matte& rhs);
		virtual SV_Matte* clone() const;

        void set_ka(const float k);
        void set_kd(const float k);
        void set_cd(const Texture* t_ptr);

        virtual RGBColor shade(ShadeRec& sr);
        virtual RGBColor area_light_shade(ShadeRec& sr);

		virtual CLMaterial get_cl_material();

    private:

        SV_Lambertian*    ambient_brdf;
        SV_Lambertian*    diffuse_brdf;
};

inline void SV_Matte::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}

inline void SV_Matte::set_kd (const float kd) {
    diffuse_brdf->set_kd(kd);
}

inline void SV_Matte::set_cd(const Texture* t_ptr) {
    ambient_brdf->set_cd((Texture*)t_ptr);
    diffuse_brdf->set_cd((Texture*)t_ptr);
}

#endif
