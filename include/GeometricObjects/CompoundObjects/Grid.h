#ifndef __GRID__
#define __GRID__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
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

		void read_flat_triangles(char* file_name);

		void read_smooth_triangles(char* file_name);

		void setup_cells();

		void reverse_mesh_normals(void);

		virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

	private:

		std::vector<GeometricObject*> cells;    // cells are stored in a 1D array
		BBox bbox;         // bounding box
		int nx, ny, nz;    // number of cells in the x-, y-, and z-directions
		Mesh* mesh_ptr;		// holds triangle data
		bool reverse_normal;	// some PLY files have normals that point inwards

		Point3D min_coordinates(); // lower corner of bbox

		Point3D max_coordinates(); // upper corner of bbox

		void read_ply_file(char* file_name, const int triangle_type);

		void compute_mesh_normals(void);
};

inline void
Grid::reverse_mesh_normals(void) {
	reverse_normal = true;
}

#endif
