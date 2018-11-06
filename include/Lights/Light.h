#ifndef __LIGHT__
#define __LIGHT__

// 	Copyright (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the declaration of the class Light

#include "Vector3D.h"
#include "RGBColor.h"
#include "ShadeRec.h"
#include "CLLight.h"

class Light {
	public:
		Light();				                // default constructor
		Light(const Light& ls);					// copy constructor
		virtual ~Light();						// deconstructor
		Light& operator= (const Light& rhs);	// assignment operator
		virtual Light* clone() const = 0;		// clone

		bool has_shadows();
		void set_shadows(bool enabled);

		virtual CLLight get_cl_light();
		virtual Vector3D get_direction(ShadeRec& sr) = 0;
		virtual RGBColor L(ShadeRec& sr) = 0;;
		virtual float G(const ShadeRec& sr) const;
		virtual float pdf(const ShadeRec& sr) const;
		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const = 0;

	protected:
		bool shadows;
};

inline void Light::set_shadows(bool enabled){
	shadows = enabled;
}

inline bool Light::has_shadows(){
	return shadows;
}

#endif
