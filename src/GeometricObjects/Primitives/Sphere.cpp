// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the definition of the class sphere

#include "Sphere.h"
#include "math.h"

const double Sphere::kEpsilon = 10e-6;

// ---------------------------------------------------------------- default constructor

Sphere::Sphere()
	: 	GeometricObject(),
		center(0.0),
		radius(1.0)
{}


// ---------------------------------------------------------------- constructor

Sphere::Sphere(Point3D c, double r)
	: 	GeometricObject(),
		center(c),
		radius(r)
{}


// ---------------------------------------------------------------- clone

Sphere*
Sphere::clone() const {
	return (new Sphere(*this));
}


// ---------------------------------------------------------------- copy constructor

Sphere::Sphere (const Sphere& sphere)
	: 	GeometricObject(sphere),
		center(sphere.center),
		radius(sphere.radius)
{}



// ---------------------------------------------------------------- assignment operator

Sphere&
Sphere::operator= (const Sphere& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center 	= rhs.center;
	radius	= rhs.radius;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Sphere::~Sphere() {}


//---------------------------------------------------------------- hit

bool
Sphere::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	double 		t;
	Vector3D	temp 	= ray.o - center;
	double 		a 		= ray.d * ray.d;
	double 		b 		= 2.0 * temp * ray.d;
	double 		c 		= temp * temp - radius * radius;
	double 		disc	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal 	 = (temp + t * ray.d) / radius;
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal   = (temp + t * ray.d) / radius;
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		}
	}

	return (false);
}

bool
Sphere::shadow_hit(const Ray& ray, float& tmin) const {
	float 		t;
	Vector3D	temp 	= ray.o - center;
	float 		a 		= ray.d * ray.d;
	float 		b 		= 2.0 * temp * ray.d;
	float 		c 		= temp * temp - radius * radius;
	float 		disc	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		float e = sqrt(disc);
		float denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			tmin = t;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = t;
			return (true);
		}
	}

	return (false);
}

BBox Sphere::get_bounding_box() {
	double delta = 0.00001;
	double x0 = center.x - radius - delta;
	double y0 = center.y - radius - delta;
	double z0 = center.z - radius - delta;
	double x1 = center.x + radius + delta;
	double y1 = center.y + radius + delta;
	double z1 = center.z + radius + delta;
	return BBox(x0, x1, y0, y1, z0, z1);
}

CLSphere Sphere::get_cl_sphere(){
	CLSphere ret;
	ret.material = material_ptr->get_cl_material();
	ret.center = (cl_double3){center.x, center.y, center.z};
	ret.radius = radius;
	return ret;
}







