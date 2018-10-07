// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class Grid
#include <algorithm>
#include <unordered_set>
#include <iostream>

#include "Grid.h"

Grid::Grid()
: Compound(), nx(0), ny(0), nz(0)
{}

Grid::Grid(const Grid& grid)
: Compound(grid) {
	cells.reserve(grid.cells.size());
	for(GeometricObject* object : grid.cells){
		if(object != NULL)
			cells.push_back(object->clone());
	}
	bbox = grid.bbox;
	nx = grid.nx;
	ny = grid.ny;
	nz = grid.nz;
}

Grid* Grid::clone() const{
	Grid* ret = new Grid(*this);
	return ret;
}

Grid::~Grid(){
	// Since objects can be repeated in cells, we'll
	// collect the unique pointers first, then delete
	// all of them
	std::unordered_set<GeometricObject*> object_ptrs;
	for(GeometricObject* object : cells){
		if(Compound* compound = dynamic_cast<Compound*>(object)) {
			for(GeometricObject* object : compound->get_objects()){
				object_ptrs.insert(object);
			}
		}
		else {
			object_ptrs.insert(object);
		}
	}
	for(GeometricObject* object : object_ptrs){
		if(object != NULL)
			delete object;
	}
	cells.clear();
}

BBox Grid::get_bounding_box(){
	return this->bbox;
}

float clamp(float x, float min, float max) {
     return (x < min ? min : (x > max ? max : x));
}

void Grid::setup_cells(void) {
	// find the minimum and maximum coordinates of the grid
	Point3D p0 = min_coordinates();
	Point3D p1 = max_coordinates();

	// store them in the bounding box

	bbox.x0 = p0.x; bbox.y0 = p0.y; bbox.z0 = p0.z;
	bbox.x1 = p1.x; bbox.y1 = p1.y; bbox.z1 = p1.z;

	// compute the number of cells in the x-, y-, and z-directions

	int num_objects = objects.size();
	float wx = p1.x - p0.x;           // grid extent in x-direction
	float wy = p1.y - p0.y;           // grid extent in y-direction
	float wz = p1.z - p0.z;           // grid extent in z-direction
	float multiplier = 2.0;           // about 8 times more cells than objects
	float s = pow(wx * wy * wz / num_objects, 0.3333333);
	nx = multiplier * wx / s + 1;
	ny = multiplier * wy / s + 1;
	nz = multiplier * wz / s + 1;

	// set up the array of cells with null pointers

	int num_cells = nx * ny * nz;
	cells.reserve(num_cells);

	for (int j = 0; j < num_cells; j++)
		cells.push_back(NULL);

	// set up a temporary array to hold the number of objects stored in each cell

	std::vector<int> counts;
	counts.reserve(num_cells);

	for (int j = 0; j < num_cells; j++)
		counts.push_back(0);

	// put objects into the cells

	BBox obj_bbox;       // object’s bounding box
	int index;           // cells array index

	for (int j = 0; j < num_objects; j++) {
		obj_bbox = objects[j]->get_bounding_box();

		// compute the cell indices for the corners of the bounding box of the
		// object

		int ixmin = clamp((obj_bbox.x0 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymin = clamp((obj_bbox.y0 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmin = clamp((obj_bbox.z0 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
		int ixmax = clamp((obj_bbox.x1 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymax = clamp((obj_bbox.y1 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmax = clamp((obj_bbox.z1 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);

		// add the object to the cells

		for (int iz = izmin; iz <= izmax; iz++)            // cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)        // cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {  // cells in x direction
					index = ix + nx * iy + nx * ny * iz;

					if (counts[index] == 0) {
						cells[index] = objects[j];
						counts[index] += 1; //index = 1
					}
					else {
						if (counts[index] == 1) {
							// construct a compound object
							Compound* compound_ptr = new Compound();
							// add the object already in cell
							compound_ptr->add_object(cells[index]);
							// add the new object
							compound_ptr->add_object(objects[j]);

							// store compound in current cell
							cells[index] = compound_ptr;
							// index = 2
							counts[index] += 1;
						}
						else {            // counts[index] > 1
							// just add current object
							cells[index]->add_object(objects[j]);

							// for statistics only
							counts[index] += 1;
						}
					}
				}
	}
	// erase Compound::Objects, but don’t delete the objects

	objects.erase(objects.begin(), objects.end());

	// code for statistics on cell objects counts can go in here

	// erase the temporary counts vector

	counts.erase(counts.begin(), counts.end());
}

bool Grid::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	// if the ray misses the grid’s bounding box
    //  return false
	double ox = ray.o.x; double oy = ray.o.y; double oz = ray.o.z;
	double dx = ray.d.x; double dy = ray.d.y; double dz = ray.d.z;

	double x0 = bbox.x0; double y0 = bbox.y0; double z0 = bbox.z0;
	double x1 = bbox.x1; double y1 = bbox.y1; double z1 = bbox.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0 = std::max(tx_min, std::max(ty_min, tz_min));
	double t1 = std::min(tx_max, std::min(ty_max, tz_max));

	if(t0 > t1 || t1 < kEpsilon){
		return false;
	}

	// if the ray starts inside the grid
	//      find the cell that contains the ray origin
	// else
	//      find the cell where the ray hits the grid from the
	//         outside

	//Find the indices of the cell where we start the grid traversal
	int ix, iy, iz;

	if (bbox.inside(ray.o)) {
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		Point3D p = ray.o + t0 * ray.d;
		ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// traverse the grid
	// widths of sides of cell
	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double tx_next, ty_next, tz_next;
	int ix_step, iy_step, iz_step;
	int ix_stop, iy_stop, iz_stop;


	// The time to reach the next cell in x direction
	if(dx > 0){
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}
	if(dx == 0){
		tx_next = kHugeValue;
	}

	// The time to reach the next cell in y direction
	if(dy > 0){
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}
	if(dy == 0){
		ty_next = kHugeValue;
	}


	// The time to reach the next cell in z direction
	if(dz > 0){
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}
	if(dz == 0){
		tz_next = kHugeValue;
	}

	while (true) {
		GeometricObject* object_ptr = cells[ix + nx * iy + nx * ny * iz];

		// If the next cell in the x-direction is nearest in time, i.e. closest
		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->hit(ray, tmin, sr) && tmin < tx_next) {
				material_ptr = object_ptr->get_material();
				return (true);
			}
			// No longer need to consider this cell, step forward to nearest cell and restart loop
			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			// If the next cell in the y-direction is nearest in time, i.e. closest
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->hit(ray, tmin, sr) && tmin < ty_next) {
					material_ptr = object_ptr->get_material();
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
			// If the next cell in the y-direction is nearest in time, i.e. closest
				if (object_ptr && object_ptr->hit(ray, tmin, sr) && tmin < tz_next) {
					material_ptr = object_ptr->get_material();
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
			}
		}
	}
	return false;
}

bool Grid::shadow_hit(const Ray& ray, float& tmin) const {
	// if the ray misses the grid’s bounding box
    //  return false
	double ox = ray.o.x; double oy = ray.o.y; double oz = ray.o.z;
	double dx = ray.d.x; double dy = ray.d.y; double dz = ray.d.z;

	double x0 = bbox.x0; double y0 = bbox.y0; double z0 = bbox.z0;
	double x1 = bbox.x1; double y1 = bbox.y1; double z1 = bbox.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0 = std::max(tx_min, std::max(ty_min, tz_min));
	double t1 = std::min(tx_max, std::min(ty_max, tz_max));

	if(t0 > t1){
		return false;
	}

	// if the ray starts inside the grid
	//      find the cell that contains the ray origin
	// else
	//      find the cell where the ray hits the grid from the
	//         outside

	//Find the indices of the cell where we start the grid traversal
	int ix, iy, iz;

	if (bbox.inside(ray.o)) {
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		Point3D p = ray.o + t0 * ray.d;
		ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// traverse the grid
	// widths of sides of cell
	float dtx = (tx_max - tx_min) / nx;
	float dty = (ty_max - ty_min) / ny;
	float dtz = (tz_max - tz_min) / nz;

	float tx_next, ty_next, tz_next;
	int ix_step, iy_step, iz_step;
	int ix_stop, iy_stop, iz_stop;

	// The time to reach the next cell in x direction
	if(dx > 0){
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}
	if(dx == 0){
		tx_next = kHugeValue;
	}

	// The time to reach the next cell in y direction
	if(dy > 0){
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}
	if(dy == 0){
		ty_next = kHugeValue;
	}


	// The time to reach the next cell in z direction
	if(dz > 0){
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}
	if(dz == 0){
		tz_next = kHugeValue;
	}

	while (true) {
		GeometricObject* object_ptr = cells[ix + nx * iy + nx * ny * iz];

		// If the next cell in the x-direction is nearest in time, i.e. closest
		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < tx_next) {
				return (true);
			}
			// No longer need to consider this cell, step forward to nearest cell and restart loop
			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			// If the next cell in the y-direction is nearest in time, i.e. closest
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < ty_next) {
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
			// If the next cell in the y-direction is nearest in time, i.e. closest
				if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < tz_next) {
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
			}
		}
	}
	return false;
}

Point3D Grid::min_coordinates(){
	BBox box;
	Point3D p0(kHugeValue);

	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++) {
		box = objects[j]->get_bounding_box();

		if (box.x0 < p0.x)
			p0.x = box.x0;
		if (box.y0 < p0.y)
			p0.y = box.y0;
		if (box.z0 < p0.z)
			p0.z = box.z0;
	}

	p0.x -= kEpsilon; p0.y -= kEpsilon; p0.z -= kEpsilon;

	return (p0);
}

Point3D Grid::max_coordinates(){
	BBox box;
	Point3D p1(-kHugeValue);

	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++) {
		box = objects[j]->get_bounding_box();

		if (box.x1 > p1.x)
			p1.x = box.x1;
		if (box.y1 > p1.y)
			p1.y = box.y1;
		if (box.z1 > p1.z)
			p1.z = box.z1;
	}

	p1.x += kEpsilon; p1.y += kEpsilon; p1.z += kEpsilon;

	return (p1);
}
