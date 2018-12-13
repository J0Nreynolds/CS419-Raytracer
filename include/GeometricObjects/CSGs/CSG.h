#ifndef __CSG__
#define __CSG__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

#include <vector>
#include <boost/icl/interval_set.hpp>
#include "Normal.h"
#include "GeometricObject.h"


typedef boost::icl::interval_set<double> TIntervalSet;

class CSG : public GeometricObject{

    public:
        CSG();
		CSG(const CSG& csg);
        CSG& operator=(const CSG& rhs);
        virtual ~CSG();
		virtual CSG* clone() const = 0;

        CSG* intersect(const CSG* other) const;
        CSG* union_with(const CSG* other) const;
        CSG* subtract(const CSG* other) const;

        virtual double f(double x,  double y,  double z) = 0;
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const = 0;
		virtual bool shadow_hit(const Ray& ray, float& tmin) const = 0;

        virtual TIntervalSet hit_times(const Ray& ray) const = 0;

        virtual char get_cl_type() const;

    protected:
        CSG* left;
        CSG* right;
};

inline char CSG::get_cl_type() const {
    return 'C';
}

#endif
