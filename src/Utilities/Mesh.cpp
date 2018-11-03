// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

using namespace std;
#include "Mesh.h"

// ----------------------------------------------------------------  default constructor

Mesh::Mesh(void)
	: 	num_vertices(0),
		num_triangles(0)
{}


// ---------------------------------------------------------------- copy constructor
// this doesn't handle the vertex_faces

Mesh::Mesh (const Mesh& m)
	: 	vertices(m.vertices),
		normals(m.normals),
		u(m.u),
		v(m.v),
		num_vertices(m.num_vertices),
		num_triangles(m.num_triangles),
		cl_index(m.cl_index)
{}


// ---------------------------------------------------------------- assignment operator
// this doesn't handle the vertex_faces

Mesh&
Mesh::operator= (const Mesh& rhs) {
	if (this == &rhs)
		return (*this);

	vertices 		= rhs.vertices;
	normals  		= rhs.normals;
	u  				= rhs.u;
	v  				= rhs.v;
	num_vertices	= rhs.num_vertices;
	num_triangles	= rhs.num_triangles;
	cl_index		= rhs.cl_index;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Mesh::~Mesh(void) {}

cl_double3* Mesh::get_cl_vertices(){
	cl_double3* ret = new cl_double3[num_vertices];
	for(int i = 0 ; i < num_vertices; i++){
		ret[i] = (cl_double3){vertices[i].x, vertices[i].y, vertices[i].z};
	}
	return ret;
}

cl_double3* Mesh::get_cl_normals(){
	cl_double3* ret = new cl_double3[num_vertices];
	for(int i = 0 ; i < num_vertices; i++){
		ret[i] = (cl_double3){normals[i].x, normals[i].y, normals[i].z};
	}
	return ret;
}
