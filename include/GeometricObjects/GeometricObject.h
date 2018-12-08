#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

// 	Copyright (C) Kevin Suffern 2000-2007.
// 	Modified work (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include <math.h>  // a lot of hit functions use use maths functions

#include "Constants.h"
#include "RGBColor.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Normal.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "Material.h"
#include "BBox.h"


//----------------------------------------------------------------------------------------------------- class GeometricObject

class GeometricObject {
	public:

		GeometricObject();
		GeometricObject(const GeometricObject& object);
		virtual GeometricObject* clone() const = 0;
		virtual ~GeometricObject();

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const = 0;
		virtual bool shadow_hit(const Ray& ray, float& tmin) const = 0;

		// Allows us to ask an object for its bounding box
		virtual BBox get_bounding_box();

		// The following three functions are only required for Chapter 3

		void set_color(const RGBColor& c);
		void set_color(const float r, const float g, const float b);
		RGBColor get_color();

		// The following three functions are only required for Chapter 14 and on

		void set_material(Material* ptr);
		Material* get_material() const;
		virtual void add_object(GeometricObject* object_ptr);	// required for compound objects

		// The following four functions are only required for objects that are light sources, eg disks, rectangles, and spheres

		void set_shadows(bool enabled);
		bool has_shadows() const;

		virtual Point3D sample();
		virtual float pdf(const ShadeRec& sr);

		// The following two functions allow us to simplify the code for smooth shaded triangle meshes

		virtual Normal get_normal() const;
		virtual Normal get_normal(const Point3D& p);

		// OpenCL functions
        void set_cl_index(int idx);
		int get_cl_index() const;
		virtual char get_cl_type() const = 0;


	protected:
		bool 				shadows;
		int 				cl_index; 			// for referencing objects in OpenCL
		RGBColor 			color;				// only used for Bare Bones ray tracing
		mutable Material* 	material_ptr;
		GeometricObject& operator=(const GeometricObject& rhs);
};

// --------------------------------------------------------------------  set_color

inline void GeometricObject::set_color(const RGBColor& c) {
	color = c;
}

// --------------------------------------------------------------------  set_color

inline void GeometricObject::set_color(const float r, const float g, const float b) {
	color.r = r;
	color.b = b;
	color.g = g;
}

// --------------------------------------------------------------------  get_color

inline RGBColor GeometricObject::get_color() {
	return (color);
}

// --------------------------------------------------------------------  set_material

inline void GeometricObject::set_material(Material* ptr) {
	material_ptr = ptr;
}

// --------------------------------------------------------------------  get_material

inline Material* GeometricObject::get_material() const {
	return material_ptr;
}

// -------------------------------------------------------------------- set_shadows

inline void GeometricObject::set_shadows(bool enabled){
	shadows = enabled;
}

// -------------------------------------------------------------------- has_shadows

inline bool GeometricObject::has_shadows() const {
	return shadows;
}

// -------------------------------------------------------------------- set_cl_index

inline void GeometricObject::set_cl_index(int idx) {
	cl_index = idx;
}

// -------------------------------------------------------------------- set_cl_index

inline int GeometricObject::get_cl_index() const {
	return cl_index;
}

#endif
