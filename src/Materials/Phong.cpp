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

RGBColor Phong::shade(ShadeRec& sr) {
    Vector3D wo = -sr.ray.d;
    RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int num_lights = sr.w.lights.size();

    for (int j = 0; j < num_lights; j++) {
        Vector3D wi = sr.w.lights[j]->get_direction(sr);
        float ndotwi = sr.normal * wi;

        if (ndotwi > 0.0)
            L += (diffuse_brdf->f(sr, wo, wi) + specular_brdf->f(sr, wo, wi)) *
                sr.w.lights[j]->L(sr) * ndotwi;
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
