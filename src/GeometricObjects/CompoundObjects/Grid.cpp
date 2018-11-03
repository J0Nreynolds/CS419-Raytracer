// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class Grid
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <iostream>

#include "Grid.h"

#include "MeshTriangle.h"
#include "FlatMeshTriangle.h"
#include "SmoothMeshTriangle.h"

#include "Triangle.h"

#include "ply.h"


typedef enum {
	flat,
	smooth
} TriangleType;

Grid::Grid()
: Compound(), nx(0), ny(0), nz(0),
	mesh_ptr(new Mesh), reverse_normal(false)
{}

Grid::Grid(Mesh* _mesh_ptr)
	: 	Compound(),
		nx(0),
		ny(0),
		nz(0),
		mesh_ptr(_mesh_ptr),
		reverse_normal(false)
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
	// We don't need these anymore, they were deleted from cells
	objects.erase(objects.begin(), objects.end());
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

	bbox.x0 = p0.x;
	bbox.y0 = p0.y;
	bbox.z0 = p0.z;

	bbox.x1 = p1.x;
	bbox.y1 = p1.y;
	bbox.z1 = p1.z;

	// compute the number of cells in the x-, y-, and z-directions

	int num_objects = objects.size();

	double wx = p1.x - p0.x;           // grid extent in x-direction
	double wy = p1.y - p0.y;           // grid extent in y-direction
	double wz = p1.z - p0.z;           // grid extent in z-direction

	double multiplier = 3.0;           // about 8 times more cells than objects
	double s = pow(wx * wy * wz / num_objects, 0.3333333);
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

		int ixmin = clamp((obj_bbox.x0 - p0.x) / wx * nx , 0, nx - 1);
		int iymin = clamp((obj_bbox.y0 - p0.y) / wy * ny, 0, ny - 1);
		int izmin = clamp((obj_bbox.z0 - p0.z) / wz * nz, 0, nz - 1);
		int ixmax = clamp((obj_bbox.x1 - p0.x) / wx * nx, 0, nx - 1);
		int iymax = clamp((obj_bbox.y1 - p0.y) / wy * ny, 0, ny - 1);
		int izmax = clamp((obj_bbox.z1 - p0.z) / wz * nz, 0, nz - 1);

		// add the object to the cells

		for (int iz = izmin; iz <= izmax; iz++) {            // cells in z direction
			for (int iy = iymin; iy <= iymax; iy++) {       // cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {  // cells in x direction
					index = ix + nx * iy + nx * ny * iz;

					if (counts[index] == 0) {
						cells[index] = objects[j];
					}
                    else {
                        if (counts[index] == 1){
                            Compound* compound = new Compound;
                            compound->add_object(cells[index]);
                            cells[index] = compound;
                        }
                        // just add current object
		                cells[index]->add_object(objects[j]);
                    }
					// for statistics only
					counts[index] += 1;
				}
            }
        }
	}

	// objects.erase(objects.begin(), objects.end());

	// code for statistics on cell objects counts can go in here
	// display some statistics on counts
	// this is useful for finding out how many cells have no objects, one object, etc
	// comment this out if you don't want to use it

	int num_zeroes 	= 0;
	int num_ones 	= 0;
	int num_twos 	= 0;
	int num_threes 	= 0;
	int num_greater = 0;

	for (int j = 0; j < num_cells; j++) {
		if (counts[j] == 0)
			num_zeroes += 1;
		if (counts[j] == 1)
			num_ones += 1;
		if (counts[j] == 2)
			num_twos += 1;
		if (counts[j] == 3)
			num_threes += 1;
		if (counts[j] > 3)
			num_greater += 1;
	}

	// std::cout << "num_cells = " << num_cells << std::endl;
	// std::cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << std::endl;
	// std::cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << std::endl;

	// erase the temporary counts vector

	counts.erase(counts.begin(), counts.end());
}

bool Grid::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	// if the ray misses the grid’s bounding box
    //  return false
	double ox = ray.o.x;
	double oy = ray.o.y;
	double oz = ray.o.z;
	double dx = ray.d.x;
	double dy = ray.d.y;
	double dz = ray.d.z;

	double x0 = bbox.x0;
	double y0 = bbox.y0;
	double z0 = bbox.z0;
	double x1 = bbox.x1;
	double y1 = bbox.y1;
	double z1 = bbox.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	if (dx >= 0) {
		tx_min = (x0 - ox) / dx;
		tx_max = (x1 - ox) / dx;
	}
	else {
		tx_min = (x1 - ox) / dx;
		tx_max = (x0 - ox) / dx;
	}

	if (dy >= 0) {
		ty_min = (y0 - oy) / dy;
		ty_max = (y1 - oy) / dy;
	}
	else {
		ty_min = (y1 - oy) / dy;
		ty_max = (y0 - oy) / dy;
	}

	if (dz >= 0) {
		tz_min = (z0 - oz) / dz;
		tz_max = (z1 - oz) / dz;
	}
	else {
		tz_min = (z1 - oz) / dz;
		tz_max = (z0 - oz) / dz;
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
	// time to traverse each side of cell
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
	if(dx == 0.0){
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
	if(dy == 0.0){
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
	if(dz == 0.0){
		tz_next = kHugeValue;
	}

	int index;
	while (true) {
		index = ix + nx * iy + nx * ny * iz;
		GeometricObject* object_ptr = cells[index];

		// If the next cell in the x-direction is nearest in time, i.e. closest
		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->hit(ray, tmin, sr) && tmin < tx_next) {
    				material_ptr = object_ptr->get_material();
    				return (true);
			}
			// No longer need to consider this cell, step forward to nearest cell and restart loop
			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop){
                break;
			}
		}
        // If the next cell in the y-direction is nearest in time, i.e. closest
		else if (ty_next < tz_next) {
			if (object_ptr && object_ptr->hit(ray, tmin, sr) && tmin < ty_next) {
				material_ptr = object_ptr->get_material();
				return (true);
			}

			ty_next += dty;
			iy += iy_step;

			if (iy == iy_stop){
                break;
			}
		}
        // If the next cell in the z-direction is nearest in time, i.e. closest
		else {
			if (object_ptr && object_ptr->hit(ray, tmin, sr) && tmin < tz_next) {
				material_ptr = object_ptr->get_material();
				return (true);
			}

			tz_next += dtz;
			iz += iz_step;

			if (iz == iz_stop){
                break;
			}
        }
	}
	return false;
}

bool Grid::shadow_hit(const Ray& ray, float& tmin) const {
	// if the ray misses the grid’s bounding box
    //  return false
	double ox = ray.o.x;
	double oy = ray.o.y;
	double oz = ray.o.z;
	double dx = ray.d.x;
	double dy = ray.d.y;
	double dz = ray.d.z;

	double x0 = bbox.x0;
	double y0 = bbox.y0;
	double z0 = bbox.z0;
	double x1 = bbox.x1;
	double y1 = bbox.y1;
	double z1 = bbox.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	if (dx >= 0) {
		tx_min = (x0 - ox) / dx;
		tx_max = (x1 - ox) / dx;
	}
	else {
		tx_min = (x1 - ox) / dx;
		tx_max = (x0 - ox) / dx;
	}

	if (dy >= 0) {
		ty_min = (y0 - oy) / dy;
		ty_max = (y1 - oy) / dy;
	}
	else {
		ty_min = (y1 - oy) / dy;
		ty_max = (y0 - oy) / dy;
	}

	if (dz >= 0) {
		tz_min = (z0 - oz) / dz;
		tz_max = (z1 - oz) / dz;
	}
	else {
		tz_min = (z1 - oz) / dz;
		tz_max = (z0 - oz) / dz;
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
	// time to traverse each side of cell
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
	if(dx == 0.0){
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
	if(dy == 0.0){
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
	if(dz == 0.0){
		tz_next = kHugeValue;
	}

	int index;
	while (true) {
		index = ix + nx * iy + nx * ny * iz;
		GeometricObject* object_ptr = cells[index];

		// If the next cell in the x-direction is nearest in time, i.e. closest
		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < tx_next) {
    				return true;
			}
			// No longer need to consider this cell, step forward to nearest cell and restart loop
			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop){
                break;
			}
		}
        // If the next cell in the y-direction is nearest in time, i.e. closest
		else if (ty_next < tz_next) {
			if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < ty_next) {
				return (true);
			}

			ty_next += dty;
			iy += iy_step;

			if (iy == iy_stop){
                break;
			}
		}
        // If the next cell in the z-direction is nearest in time, i.e. closest
		else {
			if (object_ptr && object_ptr->shadow_hit(ray, tmin) && tmin < tz_next) {
				return (true);
			}

			tz_next += dtz;
			iz += iz_step;

			if (iz == iz_stop){
                break;
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

void Grid::read_obj_file(std::string file_name){
    int multiplier = 400;
    std::ifstream file(file_name);
    if(!file.is_open())
    {
      std::cout << "ERROR" << std::endl;
    }
    std::string str;
    double x, y, z;
    int idx0, idx1, idx2;
    std::string delimiter = " ";
    std::string token;
    size_t pos;
    int i;
    bool prepareFaces = false;
    int vertexCount = 0;
    int triangleCount = 0;
    while (std::getline(file, str))
    {
        if(str[0] == 'v'){ //process vertex
            pos = 0;
            i = 0;
            while (i < 4 && (pos = str.find(delimiter))) {
                token = str.substr(0, pos);
                if(i == 1){
                    x = stod(token);
                }
                else if(i == 2){
                    y = stod(token);
                }
                else if(i == 3){
                    z = stod(token);
                }
                str.erase(0, pos + delimiter.length());
                i++;
            }
            mesh_ptr->vertices.push_back(Point3D(x*multiplier,y*multiplier,z*multiplier));
            vertexCount += 1;
        }
        else if(str[0] == 'f'){ //process face
            if(!prepareFaces){
                prepareFaces = true;
                mesh_ptr->num_vertices = vertexCount;
    		  	mesh_ptr->vertex_faces.reserve(mesh_ptr->num_vertices);
    		  	std::vector<int> faceList;

    		  	for (int j = 0; j < mesh_ptr->num_vertices; j++)
    		  	    mesh_ptr->vertex_faces.push_back(faceList); // store empty lists so that we can use the [] notation below
            }
            pos = 0;
            i = 0;
            while (i < 4 && (pos = str.find(delimiter))) {
                token = str.substr(0, pos);
                if(i == 1){
                    idx0 = stoi(token) - 1;
                }
                else if(i == 2){
                    idx1 = stoi(token) - 1;
                }
                else if(i == 3){
                    idx2 = stoi(token) - 1;
                }
                str.erase(0, pos + delimiter.length());
                i++;
            }
            str.erase(0, pos + delimiter.length());
            SmoothMeshTriangle* triangle_ptr = new SmoothMeshTriangle(mesh_ptr, idx0, idx1, idx2);
            triangle_ptr->compute_normal(reverse_normal);
            objects.push_back(triangle_ptr);
			mesh_ptr->vertex_faces[idx0].push_back(triangleCount);
			mesh_ptr->vertex_faces[idx1].push_back(triangleCount);
			mesh_ptr->vertex_faces[idx2].push_back(triangleCount);
            triangleCount += 1;
        }
        else {
            continue;
        }
    }
    compute_mesh_normals();
}

// ----------------------------------------------------------------------------- compute_mesh_normals
// this computes the average normal at each vertex
// the calculation is of order(num_vertices)
// some triangles in ply files are not defined properly

void
Grid::compute_mesh_normals(void) {
	mesh_ptr->normals.reserve(mesh_ptr->num_vertices);

	for (int index = 0; index < mesh_ptr->num_vertices; index++) {   // for each vertex
		Normal normal;    // is zero at this point

		for (int j = 0; j < (int) mesh_ptr->vertex_faces[index].size(); j++)
			normal += objects[mesh_ptr->vertex_faces[index][j]]->get_normal();

		// The following code attempts to avoid (nan, nan, nan) normalised normals when all components = 0

		if (normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0)
			normal.y = 1.0;
		else
			normal.normalize();

		mesh_ptr->normals.push_back(normal);
	}

	// erase the vertex_faces arrays because we have now finished with them

	for (int index = 0; index < mesh_ptr->num_vertices; index++)
		for (int j = 0; j < (int) mesh_ptr->vertex_faces[index].size(); j++)
			mesh_ptr->vertex_faces[index].erase (mesh_ptr->vertex_faces[index].begin(), mesh_ptr->vertex_faces[index].end());

	mesh_ptr->vertex_faces.erase (mesh_ptr->vertex_faces.begin(), mesh_ptr->vertex_faces.end());

	std::cout << "finished constructing normals" << std::endl;
}
