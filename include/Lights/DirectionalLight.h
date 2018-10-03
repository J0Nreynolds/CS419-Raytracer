#ifndef __DIRECTIONAL_LIGHT__
#define __DIRECTIONAL_LIGHT__

#include "Vector3D.h"
#include "Light.h"

class DirectionalLight: public Light {
	public:
		DirectionalLight();				                	// default constructor
		DirectionalLight(Vector3D dir);						// constructor
		DirectionalLight(RGBColor color);   				// color constructor
		DirectionalLight(RGBColor color, Vector3D dir);		// color, dir constructor
		virtual ~DirectionalLight();				        // default deconstructor

		void set_direction(Vector3D dir);
		Vector3D get_direction();

		void set_color(RGBColor c);
		RGBColor get_color();

		void set_ls(float new_ls);
		float get_ls();

		virtual CLLight get_cl_light();
		virtual Vector3D get_direction(ShadeRec sr);
		virtual RGBColor L(ShadeRec sr);

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
