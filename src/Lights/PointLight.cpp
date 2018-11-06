// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class PointLight
#include "PointLight.h"
#include "World.h"

PointLight::PointLight()
: Light(), p(0), color(1.0), ls(1.0)
{}

PointLight::PointLight(Point3D pos)
: Light(), p(pos), color(1.0), ls(1.0)
{}

PointLight::PointLight(RGBColor c)
: Light(), p(0), color(c), ls(1.0)
{}

PointLight::PointLight(RGBColor c, Point3D pos)
: Light(), p(pos), color(c), ls(1.0)
{}

PointLight::~PointLight()
{}

PointLight::PointLight(const PointLight& pl)
: Light(pl), p(pl.p), color(pl.color), ls(pl.ls)
{
}

PointLight& PointLight::operator=(const PointLight& rhs){
	if (this == &rhs)
		return (*this);

	Light::operator=(rhs);

	p = rhs.p;
	color = rhs.color;
	ls = rhs.ls;

	return (*this);
}

PointLight* PointLight::clone() const{
	return (new PointLight(*this));
}


CLLight PointLight::get_cl_light()
{
	CLLight ret = Light::get_cl_light();
	ret.pos = (cl_double3){p.x, p.y, p.z};
	ret.dir = (cl_double3){0, 0, 0};
    ret.color = (cl_float3){color.r, color.g, color.b};
    ret.ls = (cl_float)ls;
	return ret;
}

RGBColor PointLight::L(ShadeRec& sr)
{
    return ls * color;
}

Vector3D PointLight::get_direction(ShadeRec& sr)
{
    return((p - sr.local_hit_point).hat());
}

bool PointLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {
	float t;
	int num_objects = sr.w.objects.size();
	float d = p.distance(ray.o);

	for (int j = 0; j < num_objects; j++)
		if (sr.w.objects[j]->shadow_hit(ray, t) && t < d)
			return (true);

	return (false);
}
