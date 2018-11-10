#ifndef __TRANSPARENT__
#define __TRANSPARENT__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "PerfectTransmitter.h"
#include "Reflective.h"

class Transparent: public Reflective {
     public:

        Transparent();
        virtual ~Transparent();
        Transparent(const Transparent& r);
        Transparent& operator=(const Transparent& rhs);
        virtual Transparent* clone() const;

        void set_kt(float kr);
        void set_ior(float ior);

        virtual RGBColor shade(ShadeRec& sr);
        virtual RGBColor area_light_shade(ShadeRec& sr);

		virtual CLMaterial get_cl_material();

     protected:

        PerfectTransmitter* specular_btdf;

};


inline void Transparent::set_kt(float kt){
    specular_btdf->set_kt(kt);
}

inline void Transparent::set_ior(float ior){
    specular_btdf->set_ior(ior);
}

#endif
