#ifndef __SMOOTH_UV_MESH_TRIANGLE__
#define __SMOOTH_UV_MESH_TRIANGLE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the declaration of the class SmoothUVMeshTriangle

#include "SmoothMeshTriangle.h"

//-------------------------------------------------------------------------------- class SmoothUVMeshTriangle

class SmoothUVMeshTriangle: public SmoothMeshTriangle {

	public:

		SmoothUVMeshTriangle();   									// Default constructor
		SmoothUVMeshTriangle(Mesh* mesh, int i0, int i1, int i2);   // argument constructor
		SmoothUVMeshTriangle(const SmoothUVMeshTriangle& object);			// Copy constructor
		virtual SmoothUVMeshTriangle* clone() const;				// Clone
		virtual	~SmoothUVMeshTriangle();							// Destructor

		SmoothUVMeshTriangle& 											// assignment operator
		operator= (const SmoothUVMeshTriangle& mesh_triangle);

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;

		virtual char get_cl_type() const;

    private:

		float interpolate_u(double beta, double gamma) const;
		float interpolate_v(double beta, double gamma) const;
};

inline char SmoothUVMeshTriangle::get_cl_type() const {
    return 'F';
}

#endif
