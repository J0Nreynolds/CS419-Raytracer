// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.
#include "Plane.h"

const double Plane::kEpsilon = 10e-6;

// ----------------------------------------------------------------------  default constructor

Plane::Plane()
	: 	GeometricObject(),
		a(0.0),
		n(0, 1, 0)
{}


// ----------------------------------------------------------------------  constructor

Plane::Plane(const Point3D& point, const Normal& normal)
	:	GeometricObject(),
		a(point),
		n(normal)
{
		n.normalize();
}


// ---------------------------------------------------------------- copy constructor

Plane::Plane(const Plane& plane)
	:	GeometricObject(plane),
		a(plane.a),
		n(plane.n)
{}


// ---------------------------------------------------------------- clone

Plane*
Plane::clone() const {
	return (new Plane(*this));
}


// ---------------------------------------------------------------- assignment operator

Plane&
Plane::operator= (const Plane& rhs)	{

	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	a = rhs.a;
	n = rhs.n;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Plane::~Plane()
{}


// ----------------------------------------------------------------- hit

bool
Plane::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	double t = (a - ray.o) * n / (ray.d * n);

	if (t > kEpsilon) {
		tmin = t;
		sr.normal = n;
		sr.local_hit_point = ray.o + t * ray.d;

		return (true);
	}

	return(false);
}

bool
Plane::shadow_hit(const Ray& ray, float& tmin) const {
	if(!has_shadows()){
		return false;
	}
	float t = (a - ray.o) * n / (ray.d * n);

	if (t > kEpsilon) {
		tmin = t;
		return (true);
	}

	return(false);
}

CLPlane Plane::get_cl_plane(){
	CLPlane ret;
	ret.a = (cl_double3){a.x, a.y, a.z};
	ret.n = (cl_double3){n.x, n.y, n.z};
	ret.material = material_ptr->get_cl_material();
	return ret;
}


