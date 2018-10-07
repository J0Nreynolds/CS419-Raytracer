#ifndef __GRID__
#define __GRID__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Compound.h"

class Grid: public Compound {
    public:

        Grid();
        Grid(const Grid& object);
        virtual Grid* clone() const;
        virtual ~Grid();

        virtual BBox get_bounding_box();

        void setup_cells();

        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        virtual bool shadow_hit(const Ray& ray, float& tmin) const;

    private:

        std::vector<GeometricObject*> cells;    // cells are stored in a 1D array
        BBox bbox;         // bounding box
        int nx, ny, nz;    // number of cells in the x-, y-, and z-directions

        Point3D            // compute minimum grid coordinates
        min_coordinates();

        Point3D            // compute maximum grid coordinates
        max_coordinates();
};

#endif
