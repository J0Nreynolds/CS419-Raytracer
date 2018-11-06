#ifndef __EMISSIVE__
#define __EMISSIVE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "RGBColor.h"
#include "Material.h"
#include "Emissive.h"

class Emissive: public Material {
     private:
        float ls;                 // radiance scaling factor
        RGBColor ce;              // color

     public:
        // constructors, set functions, etc.
        Emissive();
        virtual ~Emissive();
		Emissive(const Emissive& e);
        Emissive& operator=(const Emissive& rhs);
		virtual Emissive* clone() const;

        void scale_radiance(const float _ls);

        void set_ce(const float r, const float g, const float b);

        void set_ce(const RGBColor& c);

        virtual RGBColor get_Le(ShadeRec& sr) const;

        virtual RGBColor shade(ShadeRec& sr);

        virtual RGBColor area_light_shade(ShadeRec& sr);

		virtual CLMaterial get_cl_material();
};

inline void Emissive::scale_radiance(const float l){
    ls = l;
}

inline void Emissive::set_ce(const float r, const float g, const float b){
    ce = RGBColor(r,g,b);
}

inline void Emissive::set_ce(const RGBColor& c){
    ce = c;
}

inline RGBColor Emissive::get_Le(ShadeRec& sr) const{
    return ls*ce;
}

#endif
