// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class Triangle
#include <algorithm>
#include "CLTriangle.h"

#include "Triangle.h"

const double Triangle::kEpsilon = 10e-6;

// ---------------------------------------------------------------- default constructor

Triangle::Triangle()
	: 	GeometricObject(),
		v0(Point3D(0, 1, 0)),
		v1(Point3D(0, 0, 0)),
		v2(Point3D(1, 0, 0))
{
}


// ---------------------------------------------------------------- constructor

Triangle::Triangle(Point3D u1, Point3D u2, Point3D u3)
	: 	GeometricObject(),
		v0(u1),
		v1(u2),
		v2(u3)
{
}


// ---------------------------------------------------------------- clone

Triangle*
Triangle::clone() const {
	return (new Triangle(*this));
}


// ---------------------------------------------------------------- copy constructor

Triangle::Triangle (const Triangle& triangle)
	: 	GeometricObject(triangle),
		v0(triangle.v0),
		v1(triangle.v0),
		v2(triangle.v1)
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

	return (*this);
}


// ---------------------------------------------------------------- destructor

Triangle::~Triangle() {}


//---------------------------------------------------------------- hit

bool
Triangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	// Find plane intersection
	Vector3D v10 = Vector3D(v1 - v0);
	Vector3D v20 = Vector3D(v2 - v0);
	Vector3D n =  v10 ^ v20; // cross product gives normal of triangle's plane
	double nlen = n.length();
	double t = (v0 - ray.o) * n / (ray.d * n);
	if (t > kEpsilon) { // plane hit
		Point3D p = ray.o + t * ray.d;
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
			sr.normal = n/nlen;
			if(n * ray.d > 0){
				sr.normal = -sr.normal;
			}
			sr.local_hit_point = p;
			return (true);
		}
	}

	   return(false);
}

bool
Triangle::shadow_hit(const Ray& ray, float& tmin) const {
	// Find plane intersection
	Vector3D v10 = Vector3D(v1 - v0);
	Vector3D v20 = Vector3D(v2 - v0);
	Vector3D n =  v10 ^ v20; // cross product gives normal of triangle's plane
	float nlen = n.length();
	float t = (v0 - ray.o) * n / (ray.d * n);
	if (t > kEpsilon) { // plane hit
		Point3D p = ray.o + t * ray.d;
		Vector3D A = Vector3D(v2 - v1) ^ Vector3D(p - v1);
		Vector3D B = (-v20) ^ Vector3D(p - v2);
		Vector3D C = v10 ^ Vector3D(p - v0);
		float signA = (A * n) > 0 ? 1 : -1; // in triangle?
		float signB = (B * n) > 0 ? 1 : -1; // in triangle?
		float signC = (C * n) > 0 ? 1 : -1; // in triangle?
		float lambda0 = signA * A.length() / nlen;
		float lambda1 = signB * B.length() / nlen;
		float lambda2 = signC * C.length() / nlen;
		if(
			lambda0 <= 1 && lambda0 >= 0 &&
			lambda1 <= 1 && lambda1 >= 0 &&
			lambda2 <= 1 && lambda2 >= 0
		){                                      // triangle hit
			tmin = t;
			return (true);
		}
	}

	   return(false);
}

BBox Triangle::get_bounding_box() {
	double minx = std::min(v0.x, std::min(v1.x, v2.x)) - kEpsilon;
	double maxx = std::max(v0.x, std::max(v1.x, v2.x)) + kEpsilon;
	double miny = std::min(v0.y, std::min(v1.y, v2.y)) - kEpsilon;
	double maxy = std::max(v0.y, std::max(v1.y, v2.y)) + kEpsilon;
	double minz = std::min(v0.z, std::min(v1.z, v2.z)) - kEpsilon;
	double maxz = std::max(v0.z, std::max(v1.z, v2.z)) + kEpsilon;
	return BBox(minx, miny, minz, maxx, maxy, maxz);
}

CLTriangle Triangle::get_cl_triangle(){
	CLTriangle ret;
	ret.v0 = (cl_double3){v0.x, v0.y, v0.z};
	ret.v1 = (cl_double3){v1.x, v1.y, v1.z};
	ret.v2 = (cl_double3){v2.x, v2.y, v2.z};
	ret.material = material_ptr->get_cl_material();
	return ret;
}
