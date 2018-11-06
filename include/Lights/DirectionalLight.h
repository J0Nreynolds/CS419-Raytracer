#ifndef __DIRECTIONAL_LIGHT__
#define __DIRECTIONAL_LIGHT__

// 	Copyright (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the declaration of the class DirectionalLight

#include "Vector3D.h"
#include "Light.h"

class DirectionalLight: public Light {
	public:
		DirectionalLight();				                	// default constructor
		DirectionalLight(Vector3D dir);						// dir constructor
		DirectionalLight(RGBColor color);   				// color constructor
		DirectionalLight(RGBColor color, Vector3D dir);		// color, dir constructor
		DirectionalLight(const DirectionalLight& pl);	  	// copy constructor
		virtual ~DirectionalLight();				        // deconstructor
		DirectionalLight& operator= (const DirectionalLight& rhs); // assignment operator
		virtual DirectionalLight* clone() const;			// clone

		void set_direction(Vector3D dir);
		Vector3D get_direction();

		void set_color(RGBColor c);
		RGBColor get_color();

		void set_ls(float new_ls);
		float get_ls();

		virtual CLLight get_cl_light();
		virtual Vector3D get_direction(ShadeRec& sr);
		virtual RGBColor L(ShadeRec& sr);
		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;

	protected:
		Vector3D d;
		RGBColor color;
		float ls;
};

inline void DirectionalLight::set_direction(Vector3D dir){
	d = dir;
}

inline Vector3D DirectionalLight::get_direction(){
	return d;
}

inline void DirectionalLight::set_color(RGBColor c){
	color = c;
}

inline RGBColor DirectionalLight::get_color(){
	return color;
}

inline void DirectionalLight::set_ls(float new_ls){
	ls = new_ls;
}

inline float DirectionalLight::get_ls(){
	return ls;
}

#endif
