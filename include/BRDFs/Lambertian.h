#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "BRDF.h"

class Lambertian: public BRDF {
    public:

        Lambertian();
        Lambertian(float kd);
        Lambertian(RGBColor cd);
        Lambertian(float kd, RGBColor cd);
        virtual ~Lambertian();
		Lambertian(const Lambertian& l);
        Lambertian& operator=(const Lambertian& rhs);
		virtual Lambertian* clone() const;

        void set_kd(float kd_new);
        float get_kd();

        void set_cd(RGBColor cd_new);
        RGBColor get_cd();

        virtual RGBColor f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const;

        virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const;

        virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

        virtual CLBRDF get_cl_brdf();

    private:

        float kd;
        RGBColor cd;
};

inline void Lambertian::set_kd(float kd_new){
    kd = kd_new;
}

inline float Lambertian::get_kd(){
    return kd;
}

inline void Lambertian::set_cd(RGBColor cd_new){
    cd = cd_new;
}

inline RGBColor Lambertian::get_cd(){
    return cd;
}

#endif
