#ifndef __POINT_LIGHT__
#define __POINT_LIGHT__

#include "Vector3D.h"
#include "Light.h"

class PointLight: public Light {
	public:
		PointLight();				                	// default constructor
		PointLight(Point3D pos);						// constructor
		PointLight(RGBColor color);   				// color constructor
		PointLight(RGBColor color, Point3D pos);		// color, dir constructor
		virtual ~PointLight();				        // default deconstructor

		void set_position(Point3D pos);
		Point3D get_position();

		void set_color(RGBColor c);
		RGBColor get_color();

		void set_ls(float new_ls);
		float get_ls();

		virtual CLLight get_cl_light();
		virtual Vector3D get_direction(ShadeRec sr);
		virtual RGBColor L(ShadeRec sr);
		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;

	protected:
		Point3D p;
		RGBColor color;
		float ls;
};

inline void PointLight::set_position(Point3D pos){
	p = pos;
}

inline Point3D PointLight::get_position(){
	return p;
}
inline void PointLight::set_color(RGBColor c){
	color = c;
}

inline RGBColor PointLight::get_color(){
	return color;
}

inline void PointLight::set_ls(float new_ls){
	ls = new_ls;
}

inline float PointLight::get_ls(){
	return ls;
}

#endif
