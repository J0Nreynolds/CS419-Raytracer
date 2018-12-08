#ifndef __PLANE__
#define __PLANE__

// 	Copyright (C) Kevin Suffern 2000-2007.
// 	Modified work (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the declaration of the class Plane

#include "GeometricObject.h"
#include "CLPlane.h"

//-------------------------------------------------------------------- class Plane

class Plane: public GeometricObject {

	public:

		Plane();   												// default constructor

		Plane(const Point3D& point, const Normal& normal);			// constructor

		Plane(const Plane& plane); 									// copy constructor

		virtual Plane* 												// virtual copy constructor
		clone() const;

		Plane& 														// assignment operator
		operator= (const Plane& rhs);

		virtual														// destructor
		~Plane();

		virtual bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

		virtual bool
		shadow_hit(const Ray& ray, float& tmin) const;

		CLPlane get_cl_plane();
		virtual char get_cl_type() const;

	private:

		Point3D 	a;   				// point through which plane passes
		Normal 		n;					// normal to the plane

		static const double kEpsilon;   // for shadows and secondary rays
};

inline char Plane::get_cl_type() const {
	return 'p';
}

#endif

