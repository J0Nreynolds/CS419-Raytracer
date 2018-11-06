#ifndef __AMBIENT__
#define __AMBIENT__

// 	Copyright (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the declaration of the class Ambient

#include "Vector3D.h"
#include "Light.h"

class Ambient: public Light {
	public:
		Ambient();								// default constructor
		Ambient(RGBColor color);				// color constructor
		Ambient(const Ambient& a);				// copy constructor
		virtual ~Ambient();						// deconstructor
		Ambient& operator= (const Ambient& rhs);// assignment operator
		virtual Ambient* clone() const;			// clone

		void set_color(RGBColor c);
		RGBColor get_color();

		void set_ls(float new_ls);
		float get_ls();

		virtual CLLight get_cl_light();
		virtual Vector3D get_direction(ShadeRec& sr);
		virtual RGBColor L(ShadeRec& sr);
		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;

	protected:
		RGBColor color;
		float ls;
};

inline void Ambient::set_color(RGBColor c){
	color = c;
}

inline RGBColor Ambient::get_color(){
	return color;
}

inline void Ambient::set_ls(float new_ls){
	ls = new_ls;
}

inline float Ambient::get_ls(){
	return ls;
}

#endif
