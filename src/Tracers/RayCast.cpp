
// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "ShadeRec.h"
#include "Material.h"
#include "World.h"
#include "RayCast.h"

// -------------------------------------------------------------------- default constructor

RayCast::RayCast()
	: Tracer()
{}


// -------------------------------------------------------------------- constructor

RayCast::RayCast(World* _worldPtr)
	: Tracer(_worldPtr)
{}



// -------------------------------------------------------------------- destructor

RayCast::~RayCast()
{}


// -------------------------------------------------------------------- trace_ray

RGBColor
RayCast::trace_ray(const Ray& ray) const {
    ShadeRec sr(world_ptr->hit_objects(ray));    // copy constructor

    if (sr.hit_an_object) {
        sr.ray = ray;          // for specular reflection (Chapter 15)
        return (sr.material_ptr->shade(sr));
    }
    else
        return (world_ptr->background_color);
}


// -------------------------------------------------------------------- trace_ray

RGBColor
RayCast::trace_ray(const Ray ray, const int depth) const {
    return trace_ray(ray);
}


// -------------------------------------------------------------------- trace_ray

RGBColor
RayCast::trace_ray(const Ray ray, float& tmin, const int depth) const {
    return trace_ray(ray);
}
