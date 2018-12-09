// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class SmoothUVMeshTriangle

#include "SmoothUVMeshTriangle.h"

SmoothUVMeshTriangle::SmoothUVMeshTriangle()
: SmoothMeshTriangle()
{}

SmoothUVMeshTriangle::SmoothUVMeshTriangle(Mesh* mesh, int i0, int i1, int i2)
: SmoothMeshTriangle(mesh, i0, i1, i2)
{}

SmoothUVMeshTriangle::SmoothUVMeshTriangle(const SmoothUVMeshTriangle& mesh_triangle)
: SmoothMeshTriangle(mesh_triangle) {
}

SmoothUVMeshTriangle* SmoothUVMeshTriangle::clone() const {
    SmoothUVMeshTriangle* ret = new SmoothUVMeshTriangle(*this);
    return ret;
}

SmoothUVMeshTriangle::~SmoothUVMeshTriangle(){
}

SmoothUVMeshTriangle& SmoothUVMeshTriangle::operator=(const SmoothUVMeshTriangle& mesh_triangle){
    SmoothMeshTriangle::operator=(mesh_triangle);
    return (*this);
}

float SmoothUVMeshTriangle::interpolate_u(double beta, double gamma) const {
    float u0(mesh_ptr->u[index0]);
    float u1(mesh_ptr->u[index1]);
    float u2(mesh_ptr->u[index2]);
    return (1 - beta - gamma) * u0 + beta * u1 + gamma * u2;
}

float SmoothUVMeshTriangle::interpolate_v(double beta, double gamma) const {
    float v0(mesh_ptr->v[index0]);
    float v1(mesh_ptr->v[index1]);
    float v2(mesh_ptr->v[index2]);
    return (1 - beta - gamma) * v0 + beta * v1 + gamma * v2;
}

bool SmoothUVMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    Point3D v0(mesh_ptr->vertices[index0]);
    Point3D v1(mesh_ptr->vertices[index1]);
    Point3D v2(mesh_ptr->vertices[index2]);

    double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
    double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
    double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, r = e * l - h * i, s = e * j - f * i;

    double inv_denom = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    double beta = e1 * inv_denom;

    if (beta < 0.0 || beta > 1.0)
      return (false);

    double e2 = a * n + d * q + c * r;
    double gamma        = e2 * inv_denom;

    if (gamma < 0.0 || gamma > 1.0)
      return (false);

    if (beta + gamma > 1.0)
      return (false);

    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < kEpsilon)
      return (false);

    tmin = t;
    sr.normal = interpolate_normal(beta, gamma); // for smooth shading
    if(sr.normal * ray.d > 0){
        sr.normal = -sr.normal;
    }
    sr.u = interpolate_u(beta, gamma); // for texture mapping
    sr.v = interpolate_v(beta, gamma);

    return(true);
}
