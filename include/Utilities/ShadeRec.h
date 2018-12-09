#ifndef __SHADE_REC__
#define __SHADE_REC__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// We can use forward references for Material and World because they are a pointer and a reference

class Material;
class World;

// We need the following as #includes instead of forward class declarations,
// because we have the objects themselves, not pointers or references to them

#include "Point3D.h"
#include "Normal.h"
#include "Ray.h"
#include "RGBColor.h"

class ShadeRec {
	public:
		bool hit_an_object;                // did the ray hit an object?
		Material* material_ptr;            // nearest object’s material
		Point3D hit_point;                 // world coordinates of hit point
		Point3D local_hit_point;           // for attaching textures to objects
		Normal normal;                     // normal at hit point
		RGBColor color;                    // only used in Chapter 3
		Ray ray;                           // for specular highlights
		int depth;                         // recursion depth
		Vector3D dir;                      // for area lights
		World& w;                          // world reference
		float u;						   // for texture coordinates
		float v;						   // for texture coordinates

		ShadeRec(World& wr);					// constructor
		ShadeRec(const ShadeRec& sr);			// copy constructor
};

#endif
