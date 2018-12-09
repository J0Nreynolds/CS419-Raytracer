#ifndef __FLAT_UV_MESH_TRIANGLE__
#define __FLAT_UV_MESH_TRIANGLE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the declaration of the class FlatUVMeshTriangle

#include "FlatMeshTriangle.h"

//-------------------------------------------------------------------------------- class FlatUVMeshTriangle

class FlatUVMeshTriangle: public FlatMeshTriangle {

	public:

		FlatUVMeshTriangle();   									// Default constructor
		FlatUVMeshTriangle(Mesh* mesh, int i0, int i1, int i2);   // argument constructor
		FlatUVMeshTriangle(const FlatUVMeshTriangle& object);			// Copy constructor
		virtual FlatUVMeshTriangle* clone() const;				// Clone
		virtual	~FlatUVMeshTriangle();							// Destructor

		FlatUVMeshTriangle& 											// assignment operator
		operator= (const FlatUVMeshTriangle& mesh_triangle);

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;

		virtual char get_cl_type() const;
};

inline char FlatUVMeshTriangle::get_cl_type() const {
    return 'F';
}

#endif
