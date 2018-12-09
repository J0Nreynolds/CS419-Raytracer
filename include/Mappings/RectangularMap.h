#ifndef __RECTANGULAR_MAP__
#define __RECTANGULAR_MAP__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Mapping.h"

class RectangularMap: public Mapping {

    public:

        RectangularMap();
        virtual ~RectangularMap();
		RectangularMap(const RectangularMap& m);
        RectangularMap& operator=(const RectangularMap& rhs);
		virtual RectangularMap* clone() const;

        virtual void get_texel_coordinates(const Point3D& local_hit_point,
            const int hres, const int vres, int& row, int& column) const;
};

#endif
