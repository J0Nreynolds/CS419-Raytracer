//  Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "World.h"
#include "EnvironmentLight.h"

EnvironmentLight::EnvironmentLight()
	: 	Light(),
		sampler_ptr(NULL),
		material_ptr(NULL)
{}

EnvironmentLight::EnvironmentLight(const EnvironmentLight& el)
	: 	Light(el) {
	if(el.sampler_ptr)
		sampler_ptr = el.sampler_ptr->clone();
	else  sampler_ptr = NULL;

	if(el.material_ptr)
		material_ptr = el.material_ptr->clone();
	else  material_ptr = NULL;
}

EnvironmentLight* EnvironmentLight::clone() const {
	return (new EnvironmentLight(*this));
}

EnvironmentLight::~EnvironmentLight() {
	// Let object deconstruct itself and material
	if(sampler_ptr){
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
	if(material_ptr){
		delete material_ptr;
		material_ptr = NULL;
	}
}

EnvironmentLight& EnvironmentLight::operator= (const EnvironmentLight& rhs) {
	if (this == &rhs)
		return (*this);

	Light::operator=(rhs);

	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}

	if (rhs.sampler_ptr)
		sampler_ptr = rhs.sampler_ptr->clone();

	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}

	if (rhs.material_ptr)
		material_ptr = rhs.material_ptr->clone();

	return (*this);
}

Vector3D EnvironmentLight::get_direction(ShadeRec& sr) {
	w = sr.normal;
	v = w ^ Vector3D(0.0072, 1.0, 0.0034);
	v.normalize();
	u = v ^ w;

	Point3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;

	return (wi);
}

RGBColor EnvironmentLight::L(ShadeRec& sr) {
	return (material_ptr->get_Le(sr));
}

bool EnvironmentLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {
	float t;
	int num_objects = sr.w.objects.size();

	for (int j = 0; j < num_objects; j++)
		if (sr.w.objects[j]->shadow_hit(ray, t))
			return (true);

	return (false);
}

float EnvironmentLight::pdf(const ShadeRec& sr) const {
	return (w * wi) / PI;
}

CLLight EnvironmentLight::get_cl_light()
{
	CLLight ret = Light::get_cl_light();
	ret.dir = (cl_double3){0, 0, 0};
	ret.pos = (cl_double3){0, 0, 0};
	ret.color = (cl_float3){0, 0, 0};
	ret.ls = (cl_float)0;
	ret.shadows = (cl_char)-2;
	return ret;
}
