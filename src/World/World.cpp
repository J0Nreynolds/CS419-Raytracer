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
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
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
#include "Grid.h"
#include "Compound.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "AmbientOccluder.h"

#include "Pinhole.h"
#include "ThinLens.h"
#include "Orthographic.h"

#include "Matte.h"
#include "Phong.h"

#include "MultipleObjects.h"
#include "RayCast.h"

World::World():
	ambient_ptr(NULL), tracer_ptr(NULL), renderer(NULL), camera_ptr(NULL), cur_cl_index(0)
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
	int num_samples = 256;

	vp.set_gamma(1.0);
	vp.set_show_out_of_gamut(false);
	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_pixel_size(1);
	vp.set_samples(num_samples);

	background_color = black;

	MultiJittered* sampler_ptr = new MultiJittered(num_samples);

	AmbientOccluder* ambient = new AmbientOccluder();
	ambient->set_color(white);
	ambient->set_min_amount(0.0);
	ambient->set_sampler(sampler_ptr);
	set_ambient_light(ambient);

	tracer_ptr = new RayCast(this);

	Pinhole* camera_ptr = new Pinhole;
	camera_ptr->set_eye(25, 20, 45);
	camera_ptr->set_lookat(0, 1, 0);
	camera_ptr->set_view_distance(5000);
	camera_ptr->compute_uvw();
	set_camera(camera_ptr);

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.75);
	matte_ptr1->set_kd(0);
	matte_ptr1->set_cd(RGBColor(1, 1, 0)); // yellow

	Sphere* sphere_ptr1 = new Sphere (Point3D(0, 1, 0), 1);
	sphere_ptr1->set_material(matte_ptr1);
	add_object(sphere_ptr1);

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.75);
	matte_ptr2->set_kd(0);
	matte_ptr2->set_cd(1); // white

	Plane* plane_ptr1 = new Plane(Point3D(0), Normal(0, 1, 0));
	plane_ptr1->set_material(matte_ptr2);
	add_object(plane_ptr1);
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
	Point3D local_hit_point;
	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.hit_point = ray.o + t * ray.d;
			sr.material_ptr = objects[j]->get_material();
			// Save for later
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
	RGBColor mapped_color;

	if (vp.show_out_of_gamut)
		mapped_color = clamp_to_color(color);
	else
		mapped_color = max_to_one(color);

	if (vp.gamma != 1.0)
		mapped_color = mapped_color.powc(vp.inv_gamma);

	int x = column;
	int y = vp.hres - row  -1;
	renderer->draw_pixel(x, y, mapped_color);
}

RGBColor World::max_to_one(const RGBColor& c) const {
	float max_value = max(c.r, max(c.g, c.b));

	if (max_value > 1.0)
		return (c / max_value);
	else
		return c;
}

RGBColor World::clamp_to_color(const RGBColor& raw_color) const {
	RGBColor c(raw_color);

	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}

	return (c);
}
