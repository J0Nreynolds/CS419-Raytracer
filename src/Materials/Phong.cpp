// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include "World.h"
#include "Phong.h"

Phong::Phong (void)
    : Material(),
    ambient_brdf(new Lambertian),
    diffuse_brdf(new Lambertian),
    specular_brdf(new GlossySpecular)
{}

Phong::~Phong()
{
    delete ambient_brdf;
    delete diffuse_brdf;
    delete specular_brdf;
}

Phong::Phong(const Phong& p)
: Material(p)
{
    if(p.ambient_brdf){
        ambient_brdf = p.ambient_brdf->clone();
    }
    else ambient_brdf = NULL;

    if(p.diffuse_brdf){
        diffuse_brdf = p.diffuse_brdf->clone();
    }
    else diffuse_brdf = NULL;

    if(p.specular_brdf){
        specular_brdf = p.specular_brdf->clone();
    }
    else specular_brdf = NULL;
}

Phong& Phong::operator=(const Phong& rhs){
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

    if(specular_brdf){
        delete specular_brdf;
        specular_brdf = NULL;
    }
    if(rhs.specular_brdf){
        specular_brdf = rhs.specular_brdf->clone();
    }

	return (*this);
}

Phong* Phong::clone() const{
    return (new Phong(*this));
}

RGBColor Phong::shade(ShadeRec& sr) {
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
                L += (diffuse_brdf->f(sr, wi, wo) + specular_brdf->f(sr, wi, wo)) *
                    sr.w.lights[j]->L(sr) * ndotwi;
        }
    }

    return (L);
}

RGBColor Phong::area_light_shade(ShadeRec& sr) {
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
                L += (diffuse_brdf->f(sr, wi, wo) + specular_brdf->f(sr, wi, wo))
                    * sr.w.lights[j]->L(sr) * sr.w.lights[j]->G(sr) * ndotwi
                    / sr.w.lights[j]->pdf(sr);
        }
    }

    return (L);
}

CLMaterial Phong::get_cl_material(){
    CLMaterial ret;
    ret.ambient_brdf = ambient_brdf->get_cl_brdf();
    ret.diffuse_brdf = diffuse_brdf->get_cl_brdf();
    ret.specular_brdf = specular_brdf->get_cl_brdf();
    return ret;
}
