// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class World
using namespace std;

#include "SDL.h"
#include <time.h>
#include <iostream>
#include <fstream>

#define __CL_ENABLE_EXCEPTIONS

#ifdef __APPLE__
#include <OpenCL/cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "Point2D.h"
#include "SDLRenderer.h"
#include "Regular.h"
#include "Jittered.h"
#include "Random.h"
#include "NRooks.h"
#include "MultiJittered.h"
#include "Hammersley.h"
#include "World.h"

#include "CLUtil.h"
#include "CLSphere.h"

#include "Triangle.h"
#include "Sphere.h"
#include "Plane.h"

#include "DirectionalLight.h"
#include "PointLight.h"

#include "Pinhole.h"
#include "ThinLens.h"
#include "Orthographic.h"

#include "Matte.h"

#include "MultipleObjects.h"
#include "RayCast.h"

World::World():
	ambient_ptr(NULL), tracer_ptr(NULL), renderer(NULL), camera_ptr(NULL)
{}

World::~World(){
	if(tracer_ptr != NULL){
		delete tracer_ptr;
		tracer_ptr = NULL;
	}
	if(renderer != NULL){
		delete renderer;
		renderer = NULL;
	}
	if(camera_ptr != NULL){
		delete camera_ptr;
		camera_ptr = NULL;
	}
	if(ambient_ptr != NULL){
		delete ambient_ptr;
		ambient_ptr = NULL;
	}
	for(GeometricObject* object: objects){
		delete object;
	}
	for(Light* light: lights){
		delete light;
	}
}

void World::build(){
	renderer = new SDLRenderer();
	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_pixel_size(0.5);
	vp.set_sampler(new MultiJittered(25));

	background_color = black;
	Ambient* ambient = new Ambient();
	set_ambient_light(ambient);
	tracer_ptr = new RayCast(this);

	// ThinLens* thinlens_ptr = new ThinLens();
	// thinlens_ptr->set_eye(0, 0, -600);
	// thinlens_ptr->set_lookat(0, 0, -50);
	// thinlens_ptr->set_view_distance(400); // set d
	// thinlens_ptr->set_lens_radius(15); // set d
	// thinlens_ptr->set_focal_plane_distance(500); // set d
	// thinlens_ptr->set_roll_angle(0); //rotate camera
	// thinlens_ptr->set_sampler(new MultiJittered(25));
	// thinlens_ptr->compute_uvw();
	// set_camera(thinlens_ptr);

	Pinhole* pinhole_ptr = new Pinhole();
	pinhole_ptr->set_eye(0, 0, -200);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(50); // set d
	pinhole_ptr->set_roll_angle(0); //rotate camera
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	// Orthographic* ortho_ptr = new Orthographic();
	// ortho_ptr->set_eye(0, 0, -600);
	// ortho_ptr->set_lookat(0, 0, -50);
	// ortho_ptr->set_roll_angle(0); //rotate camera
	// ortho_ptr->compute_uvw();
	// set_camera(ortho_ptr);

	// colors

	RGBColor yellow(1, 1, 0);										// yellow
	RGBColor brown(0.71, 0.40, 0.16);								// brown
	RGBColor dark_green(0.0, 0.41, 0.41);							// dark_green
	RGBColor orange(1.0, 0.75, 0.0);								// orange
	RGBColor dark_orange(0.7, 0.55, 0.0);								// orange
	RGBColor green(0.0, 0.6, 0.3);									// green
	RGBColor blue(0.0, 0.3, 0.9);									// green
	RGBColor dark_blue(0.0, 0.2, 0.7);									// green
	RGBColor white(1.0, 1.0, 1.0);									// green
	RGBColor light_green(0.65, 1.0, 0.30);							// light green
	RGBColor dark_yellow(0.61, 0.61, 0.0);							// dark yellow
	RGBColor light_purple(0.65, 0.3, 1.0);							// light purple
	RGBColor dark_purple(0.5, 0.0, 1.0);							// dark purple

	DirectionalLight* l1 = new DirectionalLight(Vector3D(0, 0, 1));
	add_light(l1);

	// Light* l = new PointLight(Point3D(0,0,-40));
	// add_light(l);

	// spheres
	Matte* red_material = new Matte();
	red_material->set_kd(0.65);
	red_material->set_ka(0.25);
	red_material->set_cd(red);
	for(int i = 0; i <  vp.hres; i += 40){
		for(int j = 0; j < vp.vres; j+= 40){
			Sphere*	sphere_ptr = new Sphere(Point3D(5+(2*i-400), 5+(2*j-400), 0), 35);
			sphere_ptr->set_material(red_material);	   								// yellow
			add_object(sphere_ptr);
		}
	}
}


ShadeRec World::hit_bare_bones_objects(const Ray& ray) const{
	ShadeRec sr((World&)*this);
	double tmin = kHugeValue;
	double t = tmin;
	int num_objects = objects.size();
	int num_lights = lights.size();

	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.color = objects[j]->get_color();
		}

	//calculate shading
	if(sr.hit_an_object){
		RGBColor temp = black;
		double kd = 0.8;
		for (int j = 0; j < num_lights; j++){
			Light* l = lights[j];
			Vector3D L = l->get_direction(sr);
			L.normalize();
			Vector3D N = sr.normal;
			N.normalize();
			temp += kd * max(0.0, (L * N)) * sr.color * l->L(sr);
		}
		temp /= num_lights;
		sr.color = 0.2 * sr.color + temp;
	}
	return (sr);
}

ShadeRec World::hit_objects(const Ray& ray) const {
	ShadeRec sr((World&)*this);  // constructor
	double tmin = kHugeValue;
	double t = tmin;
	Normal normal;
	Point3D local_hit_point;;
	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = objects[j]->get_material();
			sr.hit_point = ray.o + t * ray.d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}

	if (sr.hit_an_object) {
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}

	return (sr);
}

void World::open_window(const int hres, const int vres) const {
	renderer->initialize(hres, vres);
}

void World::display_pixel(	const int row,
					const int column,
					const RGBColor& color) const {
	renderer->draw_pixel(vp.vres-1-row, column, color);
}
