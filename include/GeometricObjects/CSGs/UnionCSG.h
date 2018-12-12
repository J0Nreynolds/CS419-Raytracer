#ifndef __UNION_CSG__
#define __UNION_CSG__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "CSG.h"

class UnionCSG : public CSG {

    public:
        UnionCSG();
        UnionCSG(CSG* l, CSG* r);
        virtual ~UnionCSG();
		UnionCSG(const UnionCSG& UnionCSG);
        UnionCSG& operator=(const UnionCSG& rhs);
		virtual UnionCSG* clone() const;

        virtual double f(double x,  double y,  double z);
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

        virtual std::vector<float> hit_times(const Ray& ray) const;
};

#endif
