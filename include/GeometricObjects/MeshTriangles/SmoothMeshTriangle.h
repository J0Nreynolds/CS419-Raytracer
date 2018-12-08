#ifndef __SMOOTH_MESH_TRIANGLE__
#define __SMOOTH_MESH_TRIANGLE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the declaration of the class SmoothMeshTriangle

#include "MeshTriangle.h"

//-------------------------------------------------------------------------------- class Triangle

class SmoothMeshTriangle: public MeshTriangle {

	public:

		SmoothMeshTriangle();   									// Default constructor
		SmoothMeshTriangle(Mesh* mesh, int i0, int i1, int i2);   // argument constructor
		SmoothMeshTriangle(const SmoothMeshTriangle& object);			// Copy constructor
		virtual SmoothMeshTriangle* clone() const;				// Clone
		virtual	~SmoothMeshTriangle();							// Destructor

		SmoothMeshTriangle& 											// assignment operator
		operator= (const SmoothMeshTriangle& mesh_triangle);
		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;

		virtual char get_cl_type() const;

	private:
		Normal interpolate_normal(double beta, double gamma) const;

};

inline char SmoothMeshTriangle::get_cl_type() const {
    return 'S';
}

#endif
