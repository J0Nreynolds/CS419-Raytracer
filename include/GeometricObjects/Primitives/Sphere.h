#ifndef __SPHERE__
#define __SPHERE__
// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the declaration of the class Sphere

#include "GeometricObject.h"
#include "CLSphere.h"

//-------------------------------------------------------------------------------- class Sphere

class Sphere: public GeometricObject {

	public:

		Sphere();   									// Default constructor

		Sphere(Point3D center, double r);					// Constructor

		Sphere(const Sphere& sphere); 						// Copy constructor

		virtual Sphere* 									// Virtual copy constructor
		clone() const;

		virtual												// Destructor
		~Sphere();

		Sphere& 											// assignment operator
		operator= (const Sphere& sphere);

		void
		set_center(const Point3D& c);

		void
		set_center(const double x, const double y, const double z);

		void
		set_radius(const double r);

		Point3D get_center();
		double get_radius();

		virtual bool
		hit(const Ray& ray, double& t, ShadeRec& s) const;

		virtual bool
		shadow_hit(const Ray& ray, float& tmin) const;

        virtual BBox get_bounding_box();

		CLSphere get_cl_sphere();

	private:

		Point3D 	center;   			// center coordinates as a point
		double 		radius;				// the radius

		static const double kEpsilon;   // for shadows and secondary rays
};



inline void
Sphere::set_center(const Point3D& c) {
	center = c;
}

inline void
Sphere::set_center(const double x, const double y, const double z) {
	center.x = x;
	center.y = y;
	center.z = z;
}

inline void
Sphere::set_radius(const double r) {
	radius = r;
}

inline Point3D Sphere::get_center() {
	return center;
}

inline double Sphere::get_radius() {
	return radius;
}

#endif

