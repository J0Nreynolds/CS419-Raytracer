#ifndef __MAPPING__
#define __MAPPING__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "ShadeRec.h"

class Mapping {

    public:

        Mapping();
        virtual ~Mapping();
		Mapping(const Mapping& m);
        Mapping& operator=(const Mapping& rhs);
		virtual Mapping* clone() const = 0;

        virtual void get_texel_coordinates(const Point3D& local_hit_point,
            const int hres, const int vres, int& row, int& column) const = 0;
};

#endif
