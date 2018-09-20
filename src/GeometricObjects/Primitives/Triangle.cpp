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
		v0(Point3D(0, 1, 0)),
		v1(Point3D(0, 0, 0)),
		v2(Point3D(1, 0, 0))
{
	calculate_normal();
}


// ---------------------------------------------------------------- constructor

Triangle::Triangle(Point3D u1, Point3D u2, Point3D u3)
	: 	GeometricObject(),
		v0(u1),
		v1(u2),
		v2(u3)
{
	calculate_normal();
}


// ---------------------------------------------------------------- clone

Triangle*
Triangle::clone(void) const {
	return (new Triangle(*this));
}


// ---------------------------------------------------------------- copy constructor

Triangle::Triangle (const Triangle& triangle)
	: 	GeometricObject(triangle),
		v0(triangle.v0),
		v1(triangle.v0),
		v2(triangle.v1),
		normal(triangle.normal)
{}



// ---------------------------------------------------------------- assignment operator

Triangle&
Triangle::operator= (const Triangle& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	v0 	= rhs.v0;
	v1	= rhs.v1;
	v2	= rhs.v2;
	normal = rhs.normal;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Triangle::~Triangle(void) {}

void Triangle::calculate_normal() {
	normal = Vector3D(v1 - v0) ^ Vector3D(v2 - v0);
	normal.normalize();
}


//---------------------------------------------------------------- hit

bool
Triangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	// Find plane intersection
	Vector3D v10 = Vector3D(v1 - v0);
	Vector3D v20 = Vector3D(v2 - v0);
	Vector3D n =  v10 ^ v20; // cross product gives normal of triangle's plane
	double nlen = n.length();
	float t = (v0 - ray.o) * n / (ray.d * n);
	if (t > kEpsilon && t < tmin) { // plane hit
		Point3D p = ray.o + t * ray.d;
		/**
		 * Based on fast barycentric coordinate calculation from
		 * https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
		 */
		// Vector3D e0 = Vector3D(v1 - v0);
		// Vector3D e1 = Vector3D(v2 - v0);
		// Vector3D e2 = Vector3D(p  - v0);
		// double d00 = e0 * e0;
		// double d01 = e0 * e1;
		// double d11 = e1 * e1;
		// double d20 = e2 * e0;
		// double d21 = e2 * e1;
		// double denom = d00 * d11 - d01 * d01;
		// double lambda1 = (d11 * d20 - d01 * d21) / denom;
		// double lambda2 = (d00 * d21 - d01 * d20) / denom;
		// double lambda0 = 1.0 - lambda1 - lambda2;
		Vector3D A = Vector3D(v2 - v1) ^ Vector3D(p - v1);
		Vector3D B = (-v20) ^ Vector3D(p - v2);
		Vector3D C = v10 ^ Vector3D(p - v0);
		double signA = (A * n) > 0 ? 1 : -1; // in triangle?
		double signB = (B * n) > 0 ? 1 : -1; // in triangle?
		double signC = (C * n) > 0 ? 1 : -1; // in triangle?
		double lambda0 = signA * A.length() / nlen;
		double lambda1 = signB * B.length() / nlen;
		double lambda2 = signC * C.length() / nlen;
		if(
			lambda0 <= 1 && lambda0 >= 0 &&
			lambda1 <= 1 && lambda1 >= 0 &&
			lambda2 <= 1 && lambda2 >= 0
		){                                      // triangle hit
			tmin = t;
			sr.normal = n;
			if(n * ray.d > 0){
				sr.normal = -sr.normal;
			}
			sr.local_hit_point = p;
			return (true);
		}
	}

	   return(false);
}


CLTriangle Triangle::get_cl_triangle(){
	CLTriangle ret;
	ret.v0 = (cl_double3){v0.x, v0.y, v0.z};
	ret.v1 = (cl_double3){v1.x, v1.y, v1.z};
	ret.v2 = (cl_double3){v2.x, v2.y, v2.z};
	ret.normal = (cl_double3){normal.x, normal.y, normal.z};
	return ret;
}
