// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class BRDF
#include "World.h"
#include "Matte.h"

#include <iostream>

Matte::Matte (void)
    : Material(),
    ambient_brdf(new Lambertian),
    diffuse_brdf(new Lambertian)
{}

RGBColor Matte::shade(ShadeRec& sr) {
    Vector3D wo = -sr.ray.d;
    RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int numLights = sr.w.lights.size();

    for (int j = 0; j < numLights; j++){
        Vector3D wi = sr.w.lights[j]->get_direction(sr);
        float ndotwi = sr.normal * wi;

        if (ndotwi > 0.0)
            L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
    }

    return (L);
}

CLMaterial Matte::get_cl_material(){
    CLMaterial ret;
    ret.ambient_brdf = ambient_brdf->get_cl_brdf();
    ret.diffuse_brdf = diffuse_brdf->get_cl_brdf();
    ret.specular_brdf.cd = (cl_float3){0, 0, 0};
    ret.specular_brdf.kd = 0;
    ret.specular_brdf.exp = 0;
    ret.specular_brdf.ks = 0;
    return ret;
}
