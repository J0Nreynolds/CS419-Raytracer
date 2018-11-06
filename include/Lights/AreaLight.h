#ifndef __AREA_LIGHT__
#define __AREA_LIGHT__

// 	Copyright (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the declaration of the class AreaLight


#include "GeometricObject.h"
#include "Material.h"
#include "Vector3D.h"
#include "RGBColor.h"
#include "ShadeRec.h"
#include "Light.h"
#include "CLLight.h"

class AreaLight: public Light {
     public:
		AreaLight();				      // default constructor:
   		AreaLight(const AreaLight& al);   // copy constructor
		AreaLight& operator= (const AreaLight& rhs); // assignment operator
		virtual ~AreaLight();             // deconstructor
        virtual AreaLight* clone() const; // clone

        virtual Vector3D get_direction(ShadeRec& sr);

        virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;

        virtual RGBColor L(ShadeRec& sr);

        virtual float G(const ShadeRec& sr) const;

        virtual float pdf(const ShadeRec& sr) const;

        virtual CLLight get_cl_light();

     private:

        GeometricObject* object_ptr;
        Material* material_ptr;    // an emissive material
        Point3D sample_point;      // sample point on the surface
        Normal light_normal;       // normal at sample point
        Vector3D wi;     // unit vector from hit point to sample point
};

#endif
