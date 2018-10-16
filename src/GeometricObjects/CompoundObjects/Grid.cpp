// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class Grid
#include <algorithm>
#include <unordered_set>
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

	double multiplier = 2.0;           // about 8 times more cells than objects
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

	std::cout << "num_cells = " << num_cells << std::endl;
	std::cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << std::endl;
	std::cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << std::endl;

	// erase the temporary counts vector

	counts.erase(counts.begin(), counts.end());
}

// bool Grid::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
//
// 	    double t;
// 	    Normal normal;
// 	    Point3D local_hit_point;
// 	    bool hit = false;
// 	    tmin = kHugeValue;
// 	    int num_objects = objects.size();
//
// 	    for (int j = 0; j < cells.size(); j++){
// 	        if (cells[j] && cells[j]->hit(ray, t, sr) && (t < tmin)) {
// 	            hit = true;
// 	            tmin = t;
// 	            normal = sr.normal;
// 	            local_hit_point = sr.local_hit_point;
// 	            material_ptr = cells[j]->get_material();
// 	        }
// 		}
//
// 	    if (hit) {
// 	        sr.normal = normal;
// 	        sr.local_hit_point = local_hit_point;
// 	    }
//
// 	    return (hit);
// }

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
				return (false);
			}
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

				if (iy == iy_stop){
					return (false);
				}
			}
			else {
			// If the next cell in the y-direction is nearest in time, i.e. closest
				if (object_ptr && object_ptr->hit(ray, tmin, sr) && tmin < tz_next) {
					material_ptr = object_ptr->get_material();
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop){
					return (false);
				}
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

// The following functions read a file in PLY format, and construct mesh triangles where the data is stored
// in the mesh object
// They are just small wrapper functions that call the functions read_ply_file or read_uv_ply_file that
// do the actual reading
// These use the PLY code by Greg Turk to read the PLY file


// ----------------------------------------------------------------------------- read_flat_triangles

void
Grid::read_flat_triangles(char* file_name) {
  	read_ply_file(file_name, flat);
 }


// ----------------------------------------------------------------------------- read_smooth_triangles

void
Grid::read_smooth_triangles(char* file_name) {
  	read_ply_file(file_name, smooth);
  	compute_mesh_normals();
}


// ----------------------------------------------------------------------------- read_ply_file

// Most of this function was written by Greg Turk and is released under the licence agreement
// at the start of the PLY.h and PLY.c files
// The PLY.h file is #included at the start of this file
// It still has some of his printf statements for debugging
// I've made changes to construct mesh triangles and store them in the grid
// mesh_ptr is a data member of Grid
// objects is a data member of Compound
// triangle_type is either flat or smooth
// Using the one function construct to flat and smooth triangles saves a lot of repeated code
// The ply file is the same for flat and smooth triangles


void Grid::read_ply_file(char* file_name, const int triangle_type) {
	// Vertex definition

	typedef struct Vertex {
	  float x,y,z;      // space coordinates
	} Vertex;

	// Face definition. This is the same for all files but is placed here to keep all the definitions together

	typedef struct Face {
	  	unsigned char nverts;    // number of vertex indices in list
	  	int* verts;              // vertex index list
	} Face;

	// list of property information for a vertex
	// this varies depending on what you are reading from the file

	PlyProperty vert_props[] = {
	  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
	  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
	  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0}
	};

	// list of property information for a face.
	// there is a single property, which is a list
	// this is the same for all files

	PlyProperty face_props[] = {
	  	{"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
	   		1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)}
	};

	// local variables

	int 			i,j;
  	PlyFile*		ply;
  	int 			nelems;		// number of element types: 2 in our case - vertices and faces
  	char**			elist;
	int 			file_type;
	float 			version;
	int 			nprops;		// number of properties each element has
	int 			num_elems;	// number of each type of element: number of vertices or number of faces
	PlyProperty**	plist;
	Vertex**		vlist;
	Face**			flist;
	char*			elem_name;
	int				num_comments;
	char**			comments;
	int 			num_obj_info;
	char**			obj_info;


  	// open a ply file for reading

	ply = ply_open_for_reading(file_name, &nelems, &elist, &file_type, &version);

  	// print what we found out about the file

  	printf ("version %f\n", version);
  	printf ("type %d\n", file_type);

  	// go through each kind of element that we learned is in the file and read them

  	for (i = 0; i < nelems; i++) {  // there are only two elements in our files: vertices and faces
	    // get the description of the first element

  	    elem_name = elist[i];
	    plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

	    // print the name of the element, for debugging

		std::cout << "element name  " << elem_name << "  num elements = " << num_elems << "  num properties =  " << nprops << std::endl;

	    // if we're on vertex elements, read in the properties

    	if (equal_strings ("vertex", elem_name)) {
	      	// set up for getting vertex elements
	      	// the three properties are the vertex coordinates

			ply_get_property (ply, elem_name, &vert_props[0]);
	      	ply_get_property (ply, elem_name, &vert_props[1]);
		  	ply_get_property (ply, elem_name, &vert_props[2]);

		  	// reserve mesh elements

		  	mesh_ptr->num_vertices = num_elems;
		  	mesh_ptr->vertices.reserve(num_elems);

		  	// grab all the vertex elements

		  	for (j = 0; j < num_elems; j++) {
				Vertex* vertex_ptr = new Vertex;

		        // grab an element from the file

				ply_get_element (ply, (void *) vertex_ptr);
		  		mesh_ptr->vertices.push_back(Point3D(vertex_ptr->x, vertex_ptr->y, vertex_ptr->z));
		  		delete vertex_ptr;
		  	}
    	}

	    // if we're on face elements, read them in

	    if (equal_strings ("face", elem_name)) {
		    // set up for getting face elements

			ply_get_property (ply, elem_name, &face_props[0]);   // only one property - a list

		  	mesh_ptr->num_triangles = num_elems;
		  	objects.reserve(num_elems);  // triangles will be stored in Compound::objects

			// the following code stores the face numbers that are shared by each vertex

		  	mesh_ptr->vertex_faces.reserve(mesh_ptr->num_vertices);
		  	vector<int> faceList;

		  	for (j = 0; j < mesh_ptr->num_vertices; j++)
		  		mesh_ptr->vertex_faces.push_back(faceList); // store empty lists so that we can use the [] notation below

			// grab all the face elements

			int count = 0; // the number of faces read

			for (j = 0; j < num_elems; j++) {
			    // grab an element from the file

			    Face* face_ptr = new Face;

			    ply_get_element (ply, (void *) face_ptr);

			    // construct a mesh triangle of the specified type

			    if (triangle_type == flat) {
			    	FlatMeshTriangle* triangle_ptr = new FlatMeshTriangle(mesh_ptr, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
					triangle_ptr->compute_normal(reverse_normal);
					objects.push_back(triangle_ptr);
				}

			    if (triangle_type == smooth) {
			    	SmoothMeshTriangle* triangle_ptr = new SmoothMeshTriangle(mesh_ptr, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
					triangle_ptr->compute_normal(reverse_normal); 	// the "flat triangle" normal is used to compute the average normal at each mesh vertex
					objects.push_back(triangle_ptr); 				// it's quicker to do it once here, than have to do it on average 6 times in compute_mesh_normals

					// the following code stores a list of all faces that share a vertex
					// it's used for computing the average normal at each vertex in order(num_vertices) time

					mesh_ptr->vertex_faces[face_ptr->verts[0]].push_back(count);
					mesh_ptr->vertex_faces[face_ptr->verts[1]].push_back(count);
					mesh_ptr->vertex_faces[face_ptr->verts[2]].push_back(count);
					count++;
				}
			}

			if (triangle_type == flat)
				mesh_ptr->vertex_faces.erase(mesh_ptr->vertex_faces.begin(), mesh_ptr->vertex_faces.end());
	    }

	    // print out the properties we got, for debugging

	    for (j = 0; j < nprops; j++)
	    	printf ("property %s\n", plist[j]->name);

	}  // end of for (i = 0; i < nelems; i++)


	// grab and print out the comments in the file

	comments = ply_get_comments (ply, &num_comments);

	for (i = 0; i < num_comments; i++)
	    printf ("comment = '%s'\n", comments[i]);

	// grab and print out the object information

	obj_info = ply_get_obj_info (ply, &num_obj_info);

	for (i = 0; i < num_obj_info; i++)
	    printf ("obj_info = '%s'\n", obj_info[i]);

	// close the ply file

	ply_close (ply);
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

		for (int j = 0; j < mesh_ptr->vertex_faces[index].size(); j++)
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
		for (int j = 0; j < mesh_ptr->vertex_faces[index].size(); j++)
			mesh_ptr->vertex_faces[index].erase (mesh_ptr->vertex_faces[index].begin(), mesh_ptr->vertex_faces[index].end());

	mesh_ptr->vertex_faces.erase (mesh_ptr->vertex_faces.begin(), mesh_ptr->vertex_faces.end());

	std::cout << "finished constructing normals" << std::endl;
}
