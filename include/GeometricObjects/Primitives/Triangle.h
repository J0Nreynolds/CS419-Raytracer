#ifndef __TRIANGLE__
#define __TRIANGLE__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the declaration of the class Triangle

#include "GeometricObject.h"
#include "CLTriangle.h"

//-------------------------------------------------------------------------------- class Triangle

class Triangle: public GeometricObject {

	public:

		Triangle();   									// Default constructor

		Triangle(Point3D p1, Point3D p2, Point3D p3);		// Constructor

		Triangle(const Triangle& triangle); 			    // Copy constructor

		virtual Triangle* 									// Virtual copy constructor
		clone() const;

		virtual												// Destructor
		~Triangle();

		Triangle& 											// assignment operator
		operator= (const Triangle& triangle);

		void calculate_normal();

		virtual bool
		hit(const Ray& ray, double& t, ShadeRec& s) const;

		virtual bool
		shadow_hit(const Ray& ray, float& tmin) const;

        virtual BBox get_bounding_box();

		CLTriangle get_cl_triangle();
		virtual char get_cl_type() const;

	private:
		Point3D 	v0;   			// first corner of triangle
		Point3D 	v1;   			// second corner of triangle
		Point3D 	v2;   			// third corner of triangle

		static const double kEpsilon;   // for shadows and secondary rays
};

inline char Triangle::get_cl_type() const {
	return 't';
}

#endif
