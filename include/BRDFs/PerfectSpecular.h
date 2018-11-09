#ifndef __PERFECT_SPECULAR__
#define __PERFECT_SPECULAR__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "BRDF.h"

class PerfectSpecular: public BRDF {
    public:

        PerfectSpecular();
        PerfectSpecular(float kr);
        PerfectSpecular(float kr, RGBColor cr);
        virtual ~PerfectSpecular();
		PerfectSpecular(const PerfectSpecular& ps);
        PerfectSpecular& operator=(const PerfectSpecular& rhs);
		virtual PerfectSpecular* clone() const;

        void set_kr(float kr_new);
        float get_kr();

        void set_cr(RGBColor cr_new);
        RGBColor get_cr();

        virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const;

        virtual CLBRDF get_cl_brdf();

    private:

        float kr;
        RGBColor cr;
};

inline void PerfectSpecular::set_kr(float kr_new){
    kr = kr_new;
}

inline float PerfectSpecular::get_kr(){
    return kr;
}

inline void PerfectSpecular::set_cr(RGBColor cr_new){
    cr = cr_new;
}

inline RGBColor PerfectSpecular::get_cr(){
    return cr;
}

#endif
