// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class World
#include <cstdlib>
#include <time.h>
#include <iostream>

#include "Point2D.h"
#include "SingleSphere.h"
#include "MultipleObjects.h"
#include "SDLRenderer.h"
#include "World.h"


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


void World::build_red_sphere(void){
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

void World::build(void){
	renderer = new SDLRenderer();
	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_num_samples(100);
	vp.set_pixel_size(0.5);
	vp.set_sampling_type("jittered");

	background_color = black;
	tracer_ptr = new MultipleObjects(this);

	// colours

	RGBColor yellow(1, 1, 0);										// yellow
	RGBColor brown(0.71, 0.40, 0.16);								// brown
	RGBColor dark_green(0.0, 0.41, 0.41);							// dark_green
	RGBColor orange(1.0, 0.75, 0.0);								// orange
	RGBColor green(0.0, 0.6, 0.3);									// green
	RGBColor light_green(0.65, 1.0, 0.30);							// light green
	RGBColor dark_yellow(0.61, 0.61, 0.0);							// dark yellow
	RGBColor light_purple(0.65, 0.3, 1.0);							// light purple
	RGBColor dark_purple(0.5, 0.0, 1.0);							// dark purple

	// spheres

	Sphere*	sphere_ptr1 = new Sphere(Point3D(5, 3, 0), 30);
	sphere_ptr1->set_color(yellow);	   								// yellow
	add_object(sphere_ptr1);

	Sphere*	sphere_ptr2 = new Sphere(Point3D(45, -7, -60), 20);
	sphere_ptr2->set_color(brown);									// brown
	add_object(sphere_ptr2);

	Sphere*	sphere_ptr3 = new Sphere(Point3D(40, 43, -100), 17);
	sphere_ptr3->set_color(dark_green);								// dark green
	add_object(sphere_ptr3);

	Sphere*	sphere_ptr4 = new Sphere(Point3D(-20, 28, -15), 20);
	sphere_ptr4->set_color(orange);									// orange
	add_object(sphere_ptr4);

	Sphere*	sphere_ptr5 = new Sphere(Point3D(-25, -7, -35), 27);
	sphere_ptr5->set_color(green);									// green
	add_object(sphere_ptr5);

	Sphere*	sphere_ptr6 = new Sphere(Point3D(20, -27, -35), 25);
	sphere_ptr6->set_color(light_green);							// light green
	add_object(sphere_ptr6);

	Sphere*	sphere_ptr7 = new Sphere(Point3D(35, 18, -35), 22);
	sphere_ptr7->set_color(green);   								// green
	add_object(sphere_ptr7);

	Sphere*	sphere_ptr8 = new Sphere(Point3D(-57, -17, -50), 15);
	sphere_ptr8->set_color(brown);									// brown
	add_object(sphere_ptr8);

	Sphere*	sphere_ptr9 = new Sphere(Point3D(-47, 16, -80), 23);
	sphere_ptr9->set_color(light_green);							// light green
	add_object(sphere_ptr9);

	Sphere*	sphere_ptr10 = new Sphere(Point3D(-15, -32, -60), 22);
	sphere_ptr10->set_color(dark_green);     						// dark green
	add_object(sphere_ptr10);

	Sphere*	sphere_ptr11 = new Sphere(Point3D(-35, -37, -80), 22);
	sphere_ptr11->set_color(dark_yellow);							// dark yellow
	add_object(sphere_ptr11);

	Sphere*	sphere_ptr12 = new Sphere(Point3D(10, 43, -80), 22);
	sphere_ptr12->set_color(dark_yellow);							// dark yellow
	add_object(sphere_ptr12);

	Sphere*	sphere_ptr13 = new Sphere(Point3D(30, -7, -80), 10);
	sphere_ptr13->set_color(dark_yellow);
	add_object(sphere_ptr13);										// dark yellow (hidden)

	Sphere*	sphere_ptr14 = new Sphere(Point3D(-40, 48, -110), 18);
	sphere_ptr14->set_color(dark_green); 							// dark green
	add_object(sphere_ptr14);

	Sphere*	sphere_ptr15 = new Sphere(Point3D(-10, 53, -120), 18);
	sphere_ptr15->set_color(brown); 								// brown
	add_object(sphere_ptr15);

	Sphere*	sphere_ptr16 = new Sphere(Point3D(-55, -52, -100), 10);
	sphere_ptr16->set_color(light_purple);							// light purple
	add_object(sphere_ptr16);

	Sphere*	sphere_ptr17 = new Sphere(Point3D(5, -52, -100), 15);
	sphere_ptr17->set_color(brown);									// browm
	add_object(sphere_ptr17);

	Sphere*	sphere_ptr18 = new Sphere(Point3D(-20, -57, -120), 15);
	sphere_ptr18->set_color(dark_purple);							// dark purple
	add_object(sphere_ptr18);

	Sphere*	sphere_ptr19 = new Sphere(Point3D(55, -27, -100), 17);
	sphere_ptr19->set_color(dark_green);							// dark green
	add_object(sphere_ptr19);

	Sphere*	sphere_ptr20 = new Sphere(Point3D(50, -47, -120), 15);
	sphere_ptr20->set_color(brown);									// browm
	add_object(sphere_ptr20);

	Sphere*	sphere_ptr21 = new Sphere(Point3D(70, -42, -150), 10);
	sphere_ptr21->set_color(light_purple);							// light purple
	add_object(sphere_ptr21);

	Sphere*	sphere_ptr22 = new Sphere(Point3D(5, 73, -130), 12);
	sphere_ptr22->set_color(light_purple);							// light purple
	add_object(sphere_ptr22);

	Sphere*	sphere_ptr23 = new Sphere(Point3D(66, 21, -130), 13);
	sphere_ptr23->set_color(dark_purple);							// dark purple
	add_object(sphere_ptr23);

	Sphere*	sphere_ptr24 = new Sphere(Point3D(72, -12, -140), 12);
	sphere_ptr24->set_color(light_purple);							// light purple
	add_object(sphere_ptr24);

	Sphere*	sphere_ptr25 = new Sphere(Point3D(64, 5, -160), 11);
	sphere_ptr25->set_color(green);					 				// green
	add_object(sphere_ptr25);

	Sphere*	sphere_ptr26 = new Sphere(Point3D(55, 38, -160), 12);
	sphere_ptr26->set_color(light_purple);							// light purple
	add_object(sphere_ptr26);

	Sphere*	sphere_ptr27 = new Sphere(Point3D(-73, -2, -160), 12);
	sphere_ptr27->set_color(light_purple);							// light purple
	add_object(sphere_ptr27);

	Sphere*	sphere_ptr28 = new Sphere(Point3D(30, -62, -140), 15);
	sphere_ptr28->set_color(dark_purple); 							// dark purple
	add_object(sphere_ptr28);

	Sphere*	sphere_ptr29 = new Sphere(Point3D(25, 63, -140), 15);
	sphere_ptr29->set_color(dark_purple);							// dark purple
	add_object(sphere_ptr29);

	Sphere*	sphere_ptr30 = new Sphere(Point3D(-60, 46, -140), 15);
	sphere_ptr30->set_color(dark_purple); 							// dark purple
	add_object(sphere_ptr30);

	Sphere*	sphere_ptr31 = new Sphere(Point3D(-30, 68, -130), 12);
	sphere_ptr31->set_color(light_purple); 							// light purple
	add_object(sphere_ptr31);

	Sphere*	sphere_ptr32 = new Sphere(Point3D(58, 56, -180), 11);
	sphere_ptr32->set_color(green);									//  green
	add_object(sphere_ptr32);

	Sphere*	sphere_ptr33 = new Sphere(Point3D(-63, -39, -180), 11);
	sphere_ptr33->set_color(green);									// green
	add_object(sphere_ptr33);

	Sphere*	sphere_ptr34 = new Sphere(Point3D(46, 68, -200), 10);
	sphere_ptr34->set_color(light_purple);							// light purple
	add_object(sphere_ptr34);

	Sphere*	sphere_ptr35 = new Sphere(Point3D(-3, -72, -130), 12);
	sphere_ptr35->set_color(light_purple);							// light purple
	add_object(sphere_ptr35);
}


ShadeRec World::hit_bare_bones_objects(const Ray& ray) const{
	ShadeRec sr(*this);
	double t;
	double tmin = kHugeValue;
	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.color = objects[j]->get_color();
		}

	return (sr);
}

void World::render_scene(void) const {
	RGBColor pixel_color;
	Ray ray;
	double zw = 100.0; // hard wired in

	int n = (int)sqrt((float)vp.num_samples);
    Point2D pp; // sample point on a pixel

	open_window(vp.hres, vp.vres);

	ray.d = Vector3D(0, 0, -1);

	for (int r = 0; r < vp.vres; r++)               // up
		for (int c = 0; c <= vp.hres; c++) {        // across
			if(vp.sampling_type.compare("regular") == 0){
				pixel_color = black;

				for (int p = 0; p < n; p++)              // up pixel
					for (int q = 0; q < n; q++) {       // across pixel
						pp.x = vp.s * (c - 0.5 * vp.hres + (q + 0.5) / n);
						pp.y = vp.s * (r - 0.5 * vp.vres + (p + 0.5) / n);
						ray.o = Point3D(pp.x, pp.y, zw);
						pixel_color += tracer_ptr->trace_ray(ray);
					}
				pixel_color /= vp.num_samples;;
				display_pixel(r, c, pixel_color);
			}
			else if(vp.sampling_type.compare("random") == 0){
				/* initialize random seed: */
				srand(time(NULL));

				pixel_color = black;

				for (int p = 0; p < vp.num_samples; p++) {
					float rand1 = (float) rand() / RAND_MAX;
					float rand2 = (float) rand() / RAND_MAX;
					pp.x = vp.s * (c - 0.5 * vp.hres + rand1);
					pp.y = vp.s * (r - 0.5 * vp.vres + rand2);
					ray.o = Point3D(pp.x, pp.y, zw);
					pixel_color += tracer_ptr->trace_ray(ray);
				}
				pixel_color /= vp.num_samples;;
				display_pixel(r, c, pixel_color);
			}
			else if(vp.sampling_type.compare("jittered") == 0){
				pixel_color = black;

				for (int p = 0; p < n; p++)              // up pixel
					for (int q = 0; q < n; q++) {       // across pixel
						float rand1 = (float) rand() / RAND_MAX;
						float rand2 = (float) rand() / RAND_MAX;
						pp.x = vp.s * (c - 0.5 * vp.hres + (q + rand1) / n);
						pp.y = vp.s * (r - 0.5 * vp.vres + (p + rand2) / n);
						ray.o = Point3D(pp.x, pp.y, zw);
						pixel_color += tracer_ptr->trace_ray(ray);
					}
				pixel_color /= vp.num_samples;;
				display_pixel(r, c, pixel_color);
			}
			else {
				double x = vp.s * (c - 0.5 * (vp.hres - 1.0));
				double y = vp.s * (r - 0.5 * (vp.vres - 1.0));
				ray.o = Point3D(x, y, zw);
				pixel_color = tracer_ptr->trace_ray(ray);
				display_pixel(r, c, pixel_color);
			}
		}
	renderer->display();
	renderer->save_png("renders/barebones-sampling-jittered.png");
}

void World::open_window(const int hres, const int vres) const {
	renderer->initialize(hres, vres);
	renderer->display();
}

void World::display_pixel(	const int row,
					const int column,
					const RGBColor& color) const {
	renderer->draw_pixel(vp.vres-row, column, color);
}
