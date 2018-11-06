// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "ShadeRec.h"
#include "Material.h"
#include "World.h"
#include "AreaLighting.h"

// -------------------------------------------------------------------- default constructor

AreaLighting::AreaLighting()
	: Tracer()
{}


// -------------------------------------------------------------------- constructor

AreaLighting::AreaLighting(World* _worldPtr)
	: Tracer(_worldPtr)
{}



// -------------------------------------------------------------------- destructor

AreaLighting::~AreaLighting()
{}


// -------------------------------------------------------------------- trace_ray

RGBColor
AreaLighting::trace_ray(const Ray& ray) const {
	ShadeRec sr(world_ptr->hit_objects(ray));

	if (sr.hit_an_object) {
		sr.ray = ray;
		return sr.material_ptr->area_light_shade(sr);
	}
	else
		return(world_ptr->background_color);
}


// -------------------------------------------------------------------- trace_ray

RGBColor
AreaLighting::trace_ray(const Ray ray, const int depth) const {
	return trace_ray(ray);
}


// -------------------------------------------------------------------- trace_ray

RGBColor
AreaLighting::trace_ray(const Ray ray, float& tmin, const int depth) const {
	return trace_ray(ray);
}
