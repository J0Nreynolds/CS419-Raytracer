#ifndef __GRID__
#define __GRID__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include <string>
#include "Compound.h"
#include "Mesh.h"

class Grid: public Compound {
	public:

		Grid();
		Grid(Mesh* _mesh_ptr);    // Mesh constructor
		Grid(const Grid& object);
		virtual Grid* clone() const;
		virtual ~Grid();

		virtual BBox get_bounding_box();

		void read_obj_file(std::string file_name);

		void setup_cells();

		void reverse_mesh_normals(void);

		void set_mesh_ptr(Mesh* _mesh_ptr); // use this to change mesh_ptrs while reading in multiple objects

		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

	private:

		std::vector<GeometricObject*> cells;    // cells are stored in a 1D array
		BBox bbox;         // bounding box
		int nx, ny, nz;    // number of cells in the x-, y-, and z-directions
		Mesh* mesh_ptr;		// holds triangle data during file reading
		bool reverse_normal;	// some PLY files have normals that point inwards

		Point3D min_coordinates(); // lower corner of bbox

		Point3D max_coordinates(); // upper corner of bbox

		void compute_mesh_normals(void);

};

inline void
Grid::reverse_mesh_normals(void) {
	reverse_normal = !reverse_normal;
}

inline void
Grid::set_mesh_ptr(Mesh* _mesh_ptr) {
	mesh_ptr = _mesh_ptr;
}

#endif
