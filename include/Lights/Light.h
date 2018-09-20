#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "RGBColor.h"

class Light {
	public:
		Light();				                // default constructor
		Light(Vector3D d);		// constructor

		void set_direction(Vector3D dir);
		void set_color(RGBColor color);

		Vector3D get_direction();
		RGBColor get_color();

	protected:
		Vector3D d;
		RGBColor color;
};

inline void Light::set_direction(Vector3D dir){
	d = dir;
}

inline void Light::set_color(RGBColor c){
	color = c;
}

inline Vector3D Light::get_direction(){
	return d;
}

inline RGBColor Light::get_color(){
	return color;
}

#endif
