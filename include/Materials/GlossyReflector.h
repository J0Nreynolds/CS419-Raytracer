#ifndef __GLOSSY_REFLECTOR__
#define __GLOSSY_REFLECTOR__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Phong.h"

class GlossyReflector: public Phong {

    public:

        GlossyReflector();
        virtual ~GlossyReflector();
		GlossyReflector(const GlossyReflector& gr);
        GlossyReflector& operator=(const GlossyReflector& rhs);
		virtual GlossyReflector* clone() const;

        void set_samples(const int num_samples, const float exp);
        void set_sampler(Sampler* sampler, const float exp);
        void set_kr(const float k);
        void set_cr(const RGBColor c);
        void set_exponent(const float exp);

        virtual RGBColor area_light_shade(ShadeRec& sr);

		virtual CLMaterial get_cl_material();

     private:

        GlossySpecular* glossy_specular_brdf;
};

inline void GlossyReflector::set_samples(const int num_samples, const float exp) {
    glossy_specular_brdf->set_samples(num_samples, exp);
}

inline void GlossyReflector::set_sampler(Sampler* sampler, const float exp){
    glossy_specular_brdf->set_sampler(sampler, exp);
}

inline void GlossyReflector::set_kr(const float k) {
    glossy_specular_brdf->set_ks(k);
}

inline void GlossyReflector::set_cr(const RGBColor c) {
    glossy_specular_brdf->set_cs(c);
}

inline void GlossyReflector::set_exponent(const float exp) {
    glossy_specular_brdf->set_exp(exp);
}

#endif
