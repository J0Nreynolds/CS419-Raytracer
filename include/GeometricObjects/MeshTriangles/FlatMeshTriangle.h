#ifndef __FLAT_MESH_TRIANGLE__
#define __FLAT_MESH_TRIANGLE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the declaration of the class FlatMeshTriangle

#include "MeshTriangle.h"

//-------------------------------------------------------------------------------- class Triangle

class FlatMeshTriangle: public MeshTriangle {

	public:

		FlatMeshTriangle();   									// Default constructor
		FlatMeshTriangle(Mesh* mesh, int i0, int i1, int i2);   // argument constructor
		FlatMeshTriangle(const FlatMeshTriangle& object);			// Copy constructor
		virtual FlatMeshTriangle* clone() const;				// Clone
		virtual	~FlatMeshTriangle();							// Destructor

		FlatMeshTriangle& 											// assignment operator
		operator= (const FlatMeshTriangle& mesh_triangle);

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;

		virtual char get_cl_type() const;
};

inline char FlatMeshTriangle::get_cl_type() const {
    return 'F';
}

#endif
