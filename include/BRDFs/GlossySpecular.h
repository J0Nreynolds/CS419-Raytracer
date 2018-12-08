#ifndef __GLOSSY_SPECULAR__
#define __GLOSSY_SPECULAR__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "BRDF.h"
#include "Sampler.h"

class GlossySpecular: public BRDF {
    public:

        GlossySpecular();
        GlossySpecular(float ks);
        GlossySpecular(float ks, float e);
        virtual ~GlossySpecular();
		GlossySpecular(const GlossySpecular& gs);
        GlossySpecular& operator=(const GlossySpecular& rhs);
		virtual GlossySpecular* clone() const;

        void set_ks(float kd_new);
        float get_ks();

        void set_cs(RGBColor cs_new);
        RGBColor get_cs();

        void set_exp(float exp_new);
        float get_exp();

        void set_samples(const int num_samples, const float exp);
        void set_sampler(Sampler* sampler);
        RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo, float& pdf) const;

        virtual RGBColor f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const;

        virtual CLBRDF get_cl_brdf();

    private:
        RGBColor cs;
        float ks;
        float exp;
        Sampler* sampler_ptr;
};

inline void GlossySpecular::set_ks(float ks_new){
    ks = ks_new;
}

inline float GlossySpecular::get_ks(){
    return ks;
}

inline void GlossySpecular::set_cs(RGBColor cs_new){
    cs = cs_new;
}

inline RGBColor GlossySpecular::get_cs(){
    return cs;
}

inline void GlossySpecular::set_exp(float exp_new){
    exp = exp_new;
}

inline float GlossySpecular::get_exp(){
    return exp;
}

#endif
