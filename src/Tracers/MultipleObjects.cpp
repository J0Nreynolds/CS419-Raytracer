// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the definition of the class MultipleObjects
#include "MultipleObjects.h"
#include "World.h"
#include "ShadeRec.h"

// -------------------------------------------------------------------- default constructor

MultipleObjects::MultipleObjects(void)
	: Tracer()
{}


// -------------------------------------------------------------------- constructor

MultipleObjects::MultipleObjects(World* _worldPtr)
	: Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

MultipleObjects::~MultipleObjects(void) {}


// -------------------------------------------------------------------- trace_ray

RGBColor
MultipleObjects::trace_ray(const Ray& ray) const {
	ShadeRec sr(world_ptr->hit_bare_bones_objects(ray));

	if (sr.hit_an_object)
		return (sr.color);
	else
		return (world_ptr->background_color);
}
