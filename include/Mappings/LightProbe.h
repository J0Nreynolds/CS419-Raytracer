#ifndef __LIGHT_PROBE__
#define __LIGHT_PROBE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Mapping.h"

class LightProbe: public Mapping {

    public:
        const char light_probe = (char) 0;
        const char panoramic = (char) 1;

        LightProbe();
        virtual ~LightProbe();
		LightProbe(const LightProbe& m);
        LightProbe& operator=(const LightProbe& rhs);
		virtual LightProbe* clone() const;

        virtual void get_texel_coordinates(const Point3D& local_hit_point,
            const int hres, const int vres, int& row, int& column) const;

    private:
        char map_type;
};

#endif
