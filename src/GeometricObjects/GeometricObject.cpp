// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// this file contains the definition of the class GeometricObject

#include "Constants.h"
#include "GeometricObject.h"


// ---------------------------------------------------------------------- default constructor

GeometricObject::GeometricObject()
	: 	color(black), material_ptr(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

GeometricObject::GeometricObject (const GeometricObject& object)
	: 	color(object.color), material_ptr(object.material_ptr) {
}


// ---------------------------------------------------------------------- assignment operator

GeometricObject& GeometricObject::operator= (const GeometricObject& rhs) {
	if (this == &rhs)
		return (*this);

	color = rhs.color;
	material_ptr = rhs.material_ptr;

	return (*this);
}


// ---------------------------------------------------------------------- destructor

GeometricObject::~GeometricObject () {
}


// ---------------------------------------------------------------------- add_object
// required for Compound objects

void GeometricObject::add_object(GeometricObject* object_ptr) {}

// required for objects with bboxes
BBox GeometricObject::get_bounding_box() {
	return BBox(0,0,0,0,0,0);
}

// ----------------------------------------------------------------------- get_normal

Normal GeometricObject::get_normal() const{
	return (Normal());
}


// ----------------------------------------------------------------------- compute_normal

Normal GeometricObject::get_normal(const Point3D& p) {
	return (Normal());
}



// ----------------------------------------------------------------------- sample
// returns a sample point on the object for area light shading

Point3D GeometricObject::sample() {
	return (Point3D(0.0));
}


// ----------------------------------------------------------------------- pdf
// returns the probability density function for area light shading

float GeometricObject::pdf(const ShadeRec& sr) {
	return (0.0);
}
