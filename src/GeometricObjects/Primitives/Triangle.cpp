// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the definition of the class triangle
#include "CLTriangle.h"

#include "Triangle.h"

const double Triangle::kEpsilon = 10e-6;

// ---------------------------------------------------------------- default constructor

Triangle::Triangle(void)
	: 	GeometricObject(),
		p1(Point3D(0, 1, 0)),
        p2(Point3D(0, 0, 0)),
        p3(Point3D(1, 0, 0))
{}


// ---------------------------------------------------------------- constructor

Triangle::Triangle(Point3D q1, Point3D q2, Point3D q3)
	: 	GeometricObject(),
		p1(q1),
		p2(q2),
		p3(q3)
{}


// ---------------------------------------------------------------- clone

Triangle*
Triangle::clone(void) const {
	return (new Triangle(*this));
}


// ---------------------------------------------------------------- copy constructor

Triangle::Triangle (const Triangle& triangle)
	: 	GeometricObject(triangle),
		p1(triangle.p1),
		p2(triangle.p2),
		p3(triangle.p3)
{}



// ---------------------------------------------------------------- assignment operator

Triangle&
Triangle::operator= (const Triangle& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	p1 	= rhs.p1;
	p2	= rhs.p2;
	p3	= rhs.p3;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Triangle::~Triangle(void) {}


//---------------------------------------------------------------- hit

bool
Triangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    // Find plane intersection
    Vector3D p21 = Vector3D(p2 - p1);
    Vector3D p31 = Vector3D(p3 - p1);
    Vector3D n =  p21 ^ p31; // cross product gives normal of triangle's plane
    double area = n.length() / 2;
	float t = (p1 - ray.o) * n / (ray.d * n);

	if (t > kEpsilon) { // plane hit
        Point3D p = ray.o + t * ray.d;
        Vector3D diff = Vector3D(p3 - p);
        double area_A = (Vector3D(p2 - p3) ^ diff).length() / 2;
        double area_B = (p31 ^ diff).length() / 2;
        double area_C = (p21 ^ Vector3D(p2 - p)).length() / 2;
        double lambda1 = area_A / area;
        double lambda2 = area_B / area;
        double lambda3 = area_C / area;
        double lambdaSum = lambda1 + lambda2 + lambda3;
        if( lambda1 <= 1 && lambda1 >= 0 &&
            lambda2 <= 1 && lambda2 >= 0 &&
            lambda3 <= 1 && lambda3 >= 0 &&
            lambdaSum - 1 <= kEpsilon
        ){                                      // triangle hit
    		tmin = t;
    		sr.normal = n;
    		sr.local_hit_point = p;
            return (true);
        }
    }

	return(false);
}


CLTriangle Triangle::get_cl_triangle(){
	CLTriangle ret;
	ret.p1 = (cl_double3){p1.x, p1.y, p1.z};
	ret.p2 = (cl_double3){p2.x, p2.y, p2.z};
	ret.p3 = (cl_double3){p3.x, p3.y, p3.z};
	return ret;
}
