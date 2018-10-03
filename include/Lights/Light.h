#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "RGBColor.h"
#include "ShadeRec.h"
#include "CLLight.h"

class Light {
	public:
		Light();				                // default constructor
		virtual ~Light();

		bool has_shadows();
		void set_shadows(bool enabled);

		virtual CLLight get_cl_light();
		virtual Vector3D get_direction(ShadeRec sr) = 0;
		virtual RGBColor L(ShadeRec sr) = 0;

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
