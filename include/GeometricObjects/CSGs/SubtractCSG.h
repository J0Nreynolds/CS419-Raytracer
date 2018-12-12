#ifndef __SUBTRACT_CSG__
#define __SUBTRACT_CSG__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "CSG.h"

class SubtractCSG : public CSG {

    public:
        SubtractCSG();
        SubtractCSG(CSG* l, CSG* r);
        virtual ~SubtractCSG();
		SubtractCSG(const SubtractCSG& scsg);
        SubtractCSG& operator=(const SubtractCSG& rhs);
		virtual SubtractCSG* clone() const;

        virtual double f(double x,  double y,  double z);
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

        virtual std::vector<float> hit_times(const Ray& ray) const;
};

#endif
