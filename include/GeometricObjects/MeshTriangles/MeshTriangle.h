#ifndef __MESH_TRIANGLE__
#define __MESH_TRIANGLE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the declaration of the class MeshTriangle

#include "GeometricObject.h"
#include "Mesh.h"

//-------------------------------------------------------------------------------- class Triangle

class MeshTriangle: public GeometricObject {

	public:

		MeshTriangle();   									// Default constructor
		MeshTriangle(int i0, int i1, int i2, Mesh* mesh);   // argument constructor
		MeshTriangle(const MeshTriangle& object);			// Copy constructor
		virtual MeshTriangle* clone() const = 0;				// Clone
		virtual	~MeshTriangle();							// Destructor

		MeshTriangle& 										// assignment operator
		operator= (const MeshTriangle& mesh_triangle);

		void compute_normal();

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const = 0;
		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

        virtual BBox get_bounding_box();

	protected:
        Mesh* mesh_ptr;
		int index0;   			// index of first vertex
		int index1;   			// index of second vertex
		int index2;   			// index of third vertex
        Normal normal;

		static const double kEpsilon;   // for shadows and secondary rays
};

#endif
