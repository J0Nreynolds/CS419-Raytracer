#ifndef __MATERIAL__
#define __MATERIAL__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "ShadeRec.h"
#include "CLMaterial.h"

class Material {

    public:

        Material();

        virtual RGBColor shade(ShadeRec& sr);

        virtual RGBColor area_light_shade(ShadeRec& sr);

        virtual RGBColor path_shade(ShadeRec& sr);

		virtual CLMaterial get_cl_material() = 0;
};

#endif
