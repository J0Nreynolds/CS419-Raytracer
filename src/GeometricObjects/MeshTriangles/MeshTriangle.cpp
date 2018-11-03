// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class MeshTriangle
using namespace std;

#include "MeshTriangle.h"

MeshTriangle::MeshTriangle()
: GeometricObject(), mesh_ptr(NULL)
{}

MeshTriangle::MeshTriangle(Mesh* mesh, int i0, int i1, int i2)
: GeometricObject(), mesh_ptr(mesh), index0(i0), index1(i1), index2(i2)
{
}


MeshTriangle::MeshTriangle(const MeshTriangle& mesh_triangle)
: GeometricObject(mesh_triangle) {
    mesh_ptr = mesh_triangle.mesh_ptr;
    index0 = mesh_triangle.index0;
    index1 = mesh_triangle.index1;
    index2 = mesh_triangle.index2;
    normal = mesh_triangle.normal;
}

MeshTriangle::~MeshTriangle(){
}

MeshTriangle& MeshTriangle::operator=(const MeshTriangle& mesh_triangle){
    GeometricObject::operator=(mesh_triangle);
    mesh_ptr = mesh_triangle.mesh_ptr;
    index0 = mesh_triangle.index0;
    index1 = mesh_triangle.index1;
    index2 = mesh_triangle.index2;
    normal = mesh_triangle.normal;
    return (*this);
}

void MeshTriangle::compute_normal(bool reverse_normal){
    Point3D v0(mesh_ptr->vertices[index0]);
    Point3D v1(mesh_ptr->vertices[index1]);
    Point3D v2(mesh_ptr->vertices[index2]);
    normal = (v0 - v1) ^ (v0 - v2);
    normal.normalize();
    if(reverse_normal){
        normal = -normal;
    }
}

Normal MeshTriangle::get_normal() const{
    return normal;
}

BBox MeshTriangle::get_bounding_box(void) {
     Point3D v0(mesh_ptr->vertices[index0]);
     Point3D v1(mesh_ptr->vertices[index1]);
     Point3D v2(mesh_ptr->vertices[index2]);
     return(BBox(min(min(v0.x, v1.x), v2.x) - kEpsilon,
                 max(max(v0.x, v1.x), v2.x) + kEpsilon,
                 min(min(v0.y, v1.y), v2.y) - kEpsilon,
                 max(max(v0.y, v1.y), v2.y) + kEpsilon,
                 min(min(v0.z, v1.z), v2.z) - kEpsilon,
                 max(max(v0.z, v1.z), v2.z) + kEpsilon));
}

bool MeshTriangle::shadow_hit(const Ray& ray, float& tmin) const {
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
    double gamma = e2 * inv_denom;

    if (gamma < 0.0 || gamma > 1.0)
      return (false);

    if (beta + gamma > 1.0)
      return (false);

    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < kEpsilon)
      return (false);

    tmin = t;
    return true;
}

// ---------------------------------------------------------------- interpolate_u
// this is used for texture mapping in Chapter 29

float MeshTriangle::interpolate_u(const float beta, const float gamma) const {
	return(
        (1 - beta - gamma) * mesh_ptr->u[index0]
        + beta * mesh_ptr->u[index1]
		+ gamma * mesh_ptr->u[index2]
    );
}


// ---------------------------------------------------------------- interpolate_v
// this is used for texture mapping in Chapter 29

float MeshTriangle::interpolate_v(const float beta, const float gamma) const {
    return(
        (1 - beta - gamma) * mesh_ptr->v[index0]
        + beta * mesh_ptr->v[index1]
        + gamma * mesh_ptr->v[index2]
    );
}

CLMeshTriangle MeshTriangle::get_cl_mesh_triangle(){
    CLMeshTriangle ret;
    ret.material = material_ptr->get_cl_material();
    ret.normal = (cl_double3){normal.x, normal.y, normal.z};
    ret.idx0 = (cl_int)index0;
    ret.idx1 = (cl_int)index1;
    ret.idx2 = (cl_int)index2;
    ret.mesh_idx = (cl_int)mesh_ptr->get_cl_index();
    ret.is_smooth = (cl_char)true;
    return ret;
}
