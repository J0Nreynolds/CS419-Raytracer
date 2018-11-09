// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "ShadeRec.h"
#include "Material.h"
#include "World.h"
#include "Whitted.h"

// -------------------------------------------------------------------- default constructor

Whitted::Whitted()
	: Tracer()
{}


// -------------------------------------------------------------------- constructor

Whitted::Whitted(World* _worldPtr)
	: Tracer(_worldPtr)
{}



// -------------------------------------------------------------------- destructor

Whitted::~Whitted()
{}


// -------------------------------------------------------------------- trace_ray

RGBColor
Whitted::trace_ray(const Ray& ray) const {
    return trace_ray(ray, 0);
}


// -------------------------------------------------------------------- trace_ray

RGBColor
Whitted::trace_ray(const Ray ray, const int depth) const {
    if (depth > world_ptr->vp.max_depth)
        return (black);
    else {
        ShadeRec sr(world_ptr->hit_objects(ray));
        if (sr.hit_an_object) {
            sr.depth = depth;
            sr.ray = ray;

            return (sr.material_ptr->shade(sr));
        }
        else
            return (world_ptr->background_color);
    }
}


// -------------------------------------------------------------------- trace_ray

RGBColor
Whitted::trace_ray(const Ray ray, float& tmin, const int depth) const {
	return trace_ray(ray);
}
