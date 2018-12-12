#ifndef __SPHERICAL_MAP__
#define __SPHERICAL_MAP__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Mapping.h"

class SphericalMap: public Mapping {

    public:

        SphericalMap();
        virtual ~SphericalMap();
		SphericalMap(const SphericalMap& m);
        SphericalMap& operator=(const SphericalMap& rhs);
		virtual SphericalMap* clone() const;

        void set_equirectangular(bool is_eq);

        virtual void get_texel_coordinates(const Point3D& local_hit_point,
            const int hres, const int vres, int& row, int& column) const;

    private:
        bool equirectangular;
};

inline void SphericalMap::set_equirectangular(bool is_eq){
    equirectangular = is_eq;
}

#endif
