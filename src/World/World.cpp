// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class World
#include "World.h"
#include "SingleSphere.h"
#include "SDLRenderer.h"


World::World(void):
	tracer_ptr(NULL), renderer(NULL)
{
}

World::~World(void){
	if(tracer_ptr != NULL){
		delete tracer_ptr;
		tracer_ptr = NULL;
	}
	if(renderer != NULL){
		delete renderer;
		renderer = NULL;
	}
}


void World::build(void){
	vp.set_hres(200);
	vp.set_vres(200);
	vp.set_pixel_size(1.0);
	vp.set_gamma(1.0);

	background_color = black;
	renderer = new SDLRenderer();
	tracer_ptr = new SingleSphere(this);

	sphere.set_center(0.0);
	sphere.set_radius(85.0);
}

void World::render_scene(void) const {
	RGBColor pixel_color;
	Ray ray;
	double zw = 100.0; // hard wired in
	double x, y;

	open_window(vp.hres, vp.vres);

	ray.d = Vector3D(0, 0, -1);

	for (int r = 0; r < vp.vres; r++)               // up
		for (int c = 0; c <= vp.hres; c++) {        // across
			x = vp.s * (c - 0.5 * (vp.hres - 1.0));
			y = vp.s * (r - 0.5 * (vp.vres - 1.0));
			ray.o = Point3D(x, y, zw);
			pixel_color = tracer_ptr->trace_ray(ray);
			display_pixel(r, c, pixel_color);
		}
	renderer->display();
}

void World::open_window(const int hres, const int vres) const {
	renderer->initialize(hres, vres);
	renderer->display();
}

void World::display_pixel(	const int row,
					const int column,
					const RGBColor& color) const {
	renderer->draw_pixel(row, column, color);
}
