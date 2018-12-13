// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class SphereCSG
#include <algorithm>
#include "Sphere.h"
#include "SphereCSG.h"

using namespace std;
using namespace boost::icl;

SphereCSG::SphereCSG()
: CSG(), center(0.0), radius(1.0)
{}

SphereCSG::SphereCSG(Point3D c, double r)
: CSG(), center(c), radius(r)
{}

SphereCSG::SphereCSG(const SphereCSG& scsg)
: CSG(scsg), center(scsg.center), radius(scsg.radius)
{
}

SphereCSG& SphereCSG::operator= (const SphereCSG& rhs)
{
    CSG::operator=(rhs);
    center = rhs.center;
    radius = rhs.radius;
    return (*this);
}

SphereCSG* SphereCSG::clone() const
{
    return new SphereCSG(*this);
}

SphereCSG::~SphereCSG()
{}

double SphereCSG::f(double x, double y, double z){
    Vector3D diff = Point3D(x,y,z) - center;
    return pow(diff.x, 2) + pow(diff.y, 2) + pow(diff.z, 2) - radius;
}

bool SphereCSG::hit(const Ray& ray, double& t, ShadeRec& s) const{
    Sphere sphere(center, radius);
    return sphere.hit(ray, t, s);
}

bool SphereCSG::shadow_hit(const Ray& ray, float& tmin) const{
    Sphere sphere(center, radius);
    return sphere.shadow_hit(ray, tmin);
}


Normal SphereCSG::get_normal(const Point3D& p){
    Normal n = (p - center) / radius;
    return n;
}

TIntervalSet SphereCSG::hit_times(const Ray& ray) const{
    	Vector3D	temp 	= ray.o - center;
    	double 		a 		= ray.d * ray.d;
    	double 		b 		= 2.0 * temp * ray.d;
    	double 		c 		= temp * temp - radius * radius;
    	double 		disc	= b * b - 4.0 * a * c;

        TIntervalSet ret;
    	if (disc < 0.0){
            // do nothing
        }
    	else {
    		double e = sqrt(disc);
    		double denom = 2.0 * a;
    		double t1 = (-b - e) / denom;    // smaller root
    		double t2 = (-b + e) / denom;    // larger root
            ret += continuous_interval<double>(t1,t2);
    	}

    	return ret;
}
