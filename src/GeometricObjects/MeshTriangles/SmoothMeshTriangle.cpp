// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class SmoothMeshTriangle

#include "SmoothMeshTriangle.h"

SmoothMeshTriangle::SmoothMeshTriangle()
: MeshTriangle()
{}

SmoothMeshTriangle::SmoothMeshTriangle(int i0, int i1, int i2, Mesh* mesh)
: MeshTriangle(i0, i1, i2, mesh)
{}

SmoothMeshTriangle::SmoothMeshTriangle(const SmoothMeshTriangle& mesh_triangle)
: MeshTriangle(mesh_triangle) {
}

SmoothMeshTriangle* SmoothMeshTriangle::clone() const {
    SmoothMeshTriangle* ret = new SmoothMeshTriangle(*this);
    return ret;
}

SmoothMeshTriangle::~SmoothMeshTriangle(){
}

SmoothMeshTriangle& SmoothMeshTriangle::operator=(const SmoothMeshTriangle& mesh_triangle){
    MeshTriangle::operator=(mesh_triangle);
    return (*this);
}

bool SmoothMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
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
    sr.normal = (1 - beta - gamma) * n0 + beta* n1 + gamma * n2; // for smooth shading
    sr.local_hit_point = ray.o + t * ray.d;  // for texture mapping

    return(true);
}
