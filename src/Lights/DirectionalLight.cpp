// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class DirectionalLight
#include "DirectionalLight.h"
#include "World.h"

DirectionalLight::DirectionalLight()
: Light(), d(0, 0, -1), color(1.0), ls(1.0)
{}

DirectionalLight::DirectionalLight(Vector3D dir)
: Light(), d(dir), color(1.0), ls(1.0)
{}

DirectionalLight::DirectionalLight(RGBColor c)
: Light(), d(0, 0, -1), color(c), ls(1.0)
{}

DirectionalLight::DirectionalLight(RGBColor c, Vector3D dir)
: Light(), d(dir), color(c), ls(1.0)
{}

DirectionalLight::~DirectionalLight()
{}

DirectionalLight::DirectionalLight(const DirectionalLight& dl)
: Light(dl), d(dl.d), color(dl.color), ls(dl.ls)
{
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& rhs){
	if (this == &rhs)
		return (*this);

	Light::operator=(rhs);

	d = rhs.d;
	color = rhs.color;
	ls = rhs.ls;

	return (*this);
}

DirectionalLight* DirectionalLight::clone() const{
	return (new DirectionalLight(*this));
}


CLLight DirectionalLight::get_cl_light()
{
	CLLight ret = Light::get_cl_light();
	ret.pos = (cl_double3){0, 0, 0};
	ret.dir = (cl_double3){d.x, d.y, d.z};
    ret.color = (cl_float3){color.r, color.g, color.b};
    ret.ls = (cl_float)ls;
	return ret;
}

RGBColor DirectionalLight::L(ShadeRec& sr)
{
    return ls * color;
}

Vector3D DirectionalLight::get_direction(ShadeRec& sr)
{
    return -d.hat();
}

bool DirectionalLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {
	float t;
	int num_objects = sr.w.objects.size();

	for (int j = 0; j < num_objects; j++)
		if (sr.w.objects[j]->shadow_hit(ray, t))
			return (true);

	return (false);
}
