#ifndef __INTERSECT_CSG__
#define __INTERSECT_CSG__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "CSG.h"

class IntersectCSG : public CSG {

    public:
        IntersectCSG();
        IntersectCSG(CSG* l, CSG* r);
        virtual ~IntersectCSG();
		IntersectCSG(const IntersectCSG& icsg);
        IntersectCSG& operator=(const IntersectCSG& rhs);
		virtual IntersectCSG* clone() const;

        virtual double f(double x,  double y,  double z);
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

        virtual TIntervalSet hit_times(const Ray& ray) const;
};

#endif
