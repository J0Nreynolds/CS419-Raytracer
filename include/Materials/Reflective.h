#ifndef __REFLECTIVE__
#define __REFLECTIVE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "PerfectSpecular.h"
#include "Phong.h"

class Reflective: public Phong {
    public:

        Reflective();
        virtual ~Reflective();
        Reflective(const Reflective& r);
        Reflective& operator=(const Reflective& rhs);
        virtual Reflective* clone() const;

        void set_kr(const float kr);
        void set_cr(const RGBColor& c);

        virtual RGBColor shade(ShadeRec& sr);
        virtual RGBColor area_light_shade(ShadeRec& sr);

		virtual CLMaterial get_cl_material();

    protected:

        PerfectSpecular* reflective_brdf;
};

inline void Reflective::set_kr(const float kr) {
    reflective_brdf->set_kr(kr);
}
inline void Reflective::set_cr(const RGBColor& c) {
    reflective_brdf->set_cr(c);
}

#endif
