#ifndef __SPHERE_CSG__
#define __SPHERE_CSG__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "CSG.h"

class SphereCSG : public CSG {

    public:
        SphereCSG();
        SphereCSG(Point3D center, double r);
        virtual ~SphereCSG();
		SphereCSG(const SphereCSG& SphereCSG);
        SphereCSG& operator=(const SphereCSG& rhs);
		virtual SphereCSG* clone() const;

        virtual double f(double x,  double y,  double z);
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

        virtual Normal get_normal(const Point3D& p);

        virtual TIntervalSet hit_times(const Ray& ray) const;

    private:
        Point3D center;
        double radius;
};

#endif
