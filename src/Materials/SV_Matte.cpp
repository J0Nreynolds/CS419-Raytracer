// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class SV_Matte
#include "World.h"
#include "SV_Matte.h"

SV_Matte::SV_Matte (void)
    : Material(),
    ambient_brdf(new SV_Lambertian),
    diffuse_brdf(new SV_Lambertian)
{}

SV_Matte::~SV_Matte()
{
    delete ambient_brdf;
    delete diffuse_brdf;
}

SV_Matte::SV_Matte(const SV_Matte& m)
: Material(m)
{
    if(m.ambient_brdf){
        ambient_brdf = m.ambient_brdf->clone();
    }
    else ambient_brdf = NULL;

    if(m.diffuse_brdf){
        diffuse_brdf = m.diffuse_brdf->clone();
    }
    else diffuse_brdf = NULL;
}

SV_Matte& SV_Matte::operator=(const SV_Matte& rhs){
	if (this == &rhs)
		return (*this);

    Material::operator=(rhs);

    if(ambient_brdf){
        delete ambient_brdf;
        ambient_brdf = NULL;
    }
    if(rhs.ambient_brdf){
        ambient_brdf = rhs.ambient_brdf->clone();
    }

    if(diffuse_brdf){
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }
    if(rhs.diffuse_brdf){
        diffuse_brdf = rhs.diffuse_brdf->clone();
    }

	return (*this);
}

SV_Matte* SV_Matte::clone() const{
    return (new SV_Matte(*this));
}

RGBColor SV_Matte::shade(ShadeRec& sr) {
    Vector3D wo = -sr.ray.d;
    RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int num_lights = sr.w.lights.size();

    for (int j = 0; j < num_lights; j++){
        Vector3D wi = sr.w.lights[j]->get_direction(sr);
        float ndotwi = sr.normal * wi;

        if (ndotwi > 0.0){
            bool in_shadow = false;

            if (sr.w.lights[j]->casts_shadows()) {
                Ray shadow_ray(sr.hit_point, wi);
                in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr);
            }

            if (!in_shadow)
                L += diffuse_brdf->f(sr, wi, wo) * sr.w.lights[j]->L(sr) * ndotwi;
        }
    }

    return (L);
}

RGBColor SV_Matte::area_light_shade(ShadeRec& sr) {
    Vector3D wo = -sr.ray.d;
    RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int num_lights = sr.w.lights.size();

    for (int j = 0; j < num_lights; j++) {
        Vector3D wi = sr.w.lights[j]->get_direction(sr);
        float ndotwi = sr.normal * wi;

        if (ndotwi > 0.0) {
            bool in_shadow = false;

            if (sr.w.lights[j]->casts_shadows()) {
                Ray shadow_ray(sr.hit_point, wi);
                in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr);
            }

            if (!in_shadow)
                L += diffuse_brdf->f(sr, wi, wo) * sr.w.lights[j]->L(sr)
                    * sr.w.lights[j]->G(sr) * ndotwi / sr.w.lights[j]->pdf(sr);
        }
    }

    return (L);
}

CLMaterial SV_Matte::get_cl_material(){
    CLMaterial ret;
    ret.ambient_brdf = ambient_brdf->get_cl_brdf();
    ret.diffuse_brdf = diffuse_brdf->get_cl_brdf();
    ret.specular_brdf.cd = (cl_float3){0, 0, 0};
    ret.specular_brdf.kd = 0;
    ret.specular_brdf.exp = 0;
    ret.specular_brdf.ks = 0;
    return ret;
}
