// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class SphereCSG
#include <algorithm>
#include "SphereCSG.h"

using namespace std;

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
    return false;
}

bool SphereCSG::shadow_hit(const Ray& ray, float& tmin) const{
    return false;
}

vector<float> SphereCSG::hit_times(const Ray& ray) const{
    	Vector3D	temp 	= ray.o - center;
    	float 		a 		= ray.d * ray.d;
    	float 		b 		= 2.0 * temp * ray.d;
    	float 		c 		= temp * temp - radius * radius;
    	float 		disc	= b * b - 4.0 * a * c;

        vector<float> ret;
    	if (disc < 0.0){
            // do nothing
        }
    	else {
    		float e = sqrt(disc);
    		float denom = 2.0 * a;
    		float t1 = (-b - e) / denom;    // smaller root
    		float t2 = (-b + e) / denom;    // larger root
            ret.push_back(t1);
            ret.push_back(t2);
    	}

    	return ret;
}
