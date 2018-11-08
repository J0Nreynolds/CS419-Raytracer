// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class World
#include "SDL.h"
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

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

#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"

#include "MultipleObjects.h"
#include "RayCast.h"

World::World():
	ambient_ptr(NULL), tracer_ptr(NULL), renderer(NULL), camera_ptr(NULL),
	cur_mesh_cl_index(0), cur_d2_samples_cl_index(0), cur_d3_samples_cl_index(0)
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
	for(Mesh* mesh: meshes){
		delete mesh;
	}
}

void World::build(){
	background_color = black;
	renderer = new SDLRenderer;
	int num_samples = 100;

	MultiJittered* sampler_ptr = new MultiJittered(num_samples);

	vp.set_gamma(1.0);
	vp.set_show_out_of_gamut(false);
	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_pixel_size(1);
	vp.set_sampler(sampler_ptr);
	add_double2_sampler(sampler_ptr);

	tracer_ptr = new AreaLighting(this);

	MultiJittered* occ_sampler_ptr = new MultiJittered(num_samples);
	MultiJittered* area_sampler_ptr = new MultiJittered(num_samples);

	AmbientOccluder* occluder_ptr = new AmbientOccluder;
	occluder_ptr->set_color(white);
	occluder_ptr->set_ls(4);
	occluder_ptr->set_min_amount(0.15);
	occluder_ptr->set_sampler(occ_sampler_ptr);
	set_ambient_light(occluder_ptr);
	add_double3_sampler(occ_sampler_ptr);

	Pinhole* pinhole_ptr = new Pinhole();
	pinhole_ptr->set_eye(50, 50, -75);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(4500); // set d
	pinhole_ptr->set_roll_angle(0); //rotate camera
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(20.0);
	emissive_ptr->set_ce(red);

	Rectangle* rectangle_ptr = new Rectangle(Point3D(-4, 0, -4), Vector3D(2, 0, 0), Vector3D(0, 2, 0));
	rectangle_ptr->set_material(emissive_ptr);
	rectangle_ptr->set_shadows(false);
	rectangle_ptr->set_sampler(area_sampler_ptr);
	add_object(rectangle_ptr);
	add_double2_sampler(area_sampler_ptr);

	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(rectangle_ptr);
	area_light_ptr->set_shadows(true);
	// add_light(area_light_ptr);

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.15);
	matte_ptr1->set_kd(0.85);
	matte_ptr1->set_cd(RGBColor(1, 1, 0)); // blue

	Sphere* sphere_ptr1 = new Sphere (Point3D(0, 1, 0), 1);
	sphere_ptr1->set_material(matte_ptr1);
	add_object(sphere_ptr1);

	Sphere* sphere_ptr2 = new Sphere (Point3D(-2, 1, 2), 1);
	sphere_ptr2->set_material(matte_ptr1);
	add_object(sphere_ptr2);

	Sphere* sphere_ptr3 = new Sphere (Point3D(-4, 1, 4), 1);
	sphere_ptr3->set_material(matte_ptr1);
	add_object(sphere_ptr3);

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.15);
	matte_ptr2->set_kd(0.85);
	matte_ptr2->set_cd(1); // white

	Plane* plane_ptr1 = new Plane(Point3D(0), Normal(0, 1, 0));
	plane_ptr1->set_material(matte_ptr2);
	add_object(plane_ptr1);

	// MultiJittered* area_sampler_ptr1 = new MultiJittered(num_samples);
	//
	// Emissive* emissive_ptr1 = new Emissive;
	// emissive_ptr1->scale_radiance(20.0);
	// emissive_ptr1->set_ce(white);
	//
	// Rectangle* rectangle_ptr1 = new Rectangle(Point3D(-2.5, 7, 2), Vector3D(2, 0, 0),Vector3D(0, 0, 2));
	// rectangle_ptr1->set_material(emissive_ptr1);
	// rectangle_ptr1->set_shadows(false);
	// rectangle_ptr1->set_sampler(area_sampler_ptr1);
	// add_object(rectangle_ptr1);
	//
	// AreaLight* area_light_ptr1 = new AreaLight;
	// area_light_ptr1->set_object(rectangle_ptr1);
	// area_light_ptr1->set_shadows(true);
	// add_light(area_light_ptr1);

	/**
	 * BASIC SPHERE WITH AMBIENT OCCLUSION
	 */
	// tracer_ptr = new RayCast(this);
	//
	// MultiJittered* sampler_ptr = new MultiJittered(num_samples);
	//
	// AmbientOccluder* occluder_ptr = new AmbientOccluder;
	// occluder_ptr->set_color(white);
	// occluder_ptr->set_min_amount(0.0);
	// occluder_ptr->set_sampler(sampler_ptr);
	// set_ambient_light(occluder_ptr);
	//
	// Pinhole* camera_ptr = new Pinhole;
	// camera_ptr->set_eye(25, 20, 45);
	// camera_ptr->set_lookat(0, 1, 0);
	// camera_ptr->set_view_distance(5000);
	// camera_ptr->compute_uvw();
	// set_camera(camera_ptr);
	//
	// Matte* matte_ptr1 = new Matte;
	// matte_ptr1->set_ka(0.75);
	// matte_ptr1->set_kd(0);
	// matte_ptr1->set_cd(RGBColor(1, 1, 0)); // yellow
	//
	// Sphere* sphere_ptr1 = new Sphere (Point3D(0, 1, 0), 1);
	// sphere_ptr1->set_material(matte_ptr1);
	// add_object(sphere_ptr1);
	//
	// Matte* matte_ptr2 = new Matte;
	// matte_ptr2->set_ka(0.75);
	// matte_ptr2->set_kd(0);
	// matte_ptr2->set_cd(1); // white
	//
	// Plane* plane_ptr1 = new Plane(Point3D(0), Normal(0, 1, 0));
	// plane_ptr1->set_material(matte_ptr2);
	// add_object(plane_ptr1);

	/**
	 * COW MESH WITH AMBIENT OCCLUSION
	 */
	// renderer = new SDLRenderer();
	// int num_samples = 256;
	//
	// vp.set_gamma(1.0);
	// vp.set_show_out_of_gamut(false);
	// vp.set_hres(400);
	// vp.set_vres(400);
	// vp.set_pixel_size(0.5);
	// vp.set_samples(num_samples);
	//
	// background_color = black;
	//
	// MultiJittered* sampler_ptr = new MultiJittered(num_samples);
	//
	// AmbientOccluder* ambient = new AmbientOccluder();
	// ambient->set_color(white);
	// ambient->set_min_amount(0.0);
	// ambient->set_sampler(sampler_ptr);
	// set_ambient_light(ambient);
	//
	// tracer_ptr = new RayCast(this);
	//
	// // Camera
	//
	// // ThinLens* thinlens_ptr = new ThinLens();
	// // thinlens_ptr->set_eye(800, 0, 450);
	// // thinlens_ptr->set_lookat(0, 0, 0);
	// // thinlens_ptr->set_view_distance(150); // set d
	// // thinlens_ptr->set_lens_radius(50); // set d
	// // thinlens_ptr->set_focal_plane_distance(750); // set d
	// // thinlens_ptr->set_roll_angle(0); //rotate camera
	// // thinlens_ptr->set_sampler(new MultiJittered(100));
	// // thinlens_ptr->compute_uvw();
	// // set_camera(thinlens_ptr);
	//
	// Pinhole* pinhole_ptr = new Pinhole();
	// pinhole_ptr->set_eye(500, 0, 500);
	// pinhole_ptr->set_lookat(0, 0, 0);
	// pinhole_ptr->set_view_distance(150); // set d
	// pinhole_ptr->set_roll_angle(0); //rotate camera
	// pinhole_ptr->compute_uvw();
	// set_camera(pinhole_ptr);
	//
	// // Orthographic* ortho_ptr = new Orthographic();
	// // ortho_ptr->set_eye(0, 0, 250);
	// // ortho_ptr->set_lookat(0, 0, 0);
	// // ortho_ptr->set_roll_angle(0); //rotate camera
	// // ortho_ptr->compute_uvw();
	// // set_camera(ortho_ptr);
	//
	// // colors
	//
	// RGBColor yellow(1, 1, 0);										// yellow
	// RGBColor brown(0.71, 0.40, 0.16);								// brown
	// RGBColor dark_green(0.0, 0.41, 0.41);							// dark_green
	// RGBColor orange(1.0, 0.75, 0.0);								// orange
	// RGBColor dark_orange(0.7, 0.55, 0.0);								// orange
	// RGBColor green(0.0, 0.6, 0.3);									// green
	// RGBColor blue(0.0, 0.3, 0.9);									// green
	// RGBColor dark_blue(0.0, 0.2, 0.7);									// green
	// RGBColor white(1.0, 1.0, 1.0);									// green
	// RGBColor light_green(0.65, 1.0, 0.30);							// light green
	// RGBColor dark_yellow(0.61, 0.61, 0.0);							// dark yellow
	// RGBColor light_purple(0.65, 0.3, 1.0);							// light purple
	// RGBColor dark_purple(0.5, 0.0, 1.0);							// dark purple
	//
	// // Lights
	//
	// // DirectionalLight* l1 = new DirectionalLight(Vector3D(0, 0, -1));
	// // l1->set_shadows(false);
	// // l1->set_ls(5.0);
	// // add_light(l1);
	//
	PointLight* l = new PointLight(Point3D(300,50,500));
	l->set_shadows(true);
	l->set_ls(3.0);
	add_light(l);
	//
	// Phong* orange_material = new Phong();
	// orange_material->set_ks(0.15);
	// orange_material->set_exp(50);
	// orange_material->set_kd(0.6);
	// orange_material->set_ka(0.25);
	// orange_material->set_cd(orange);
	//
	// // Plane
	// Plane* plane_ptr = new Plane(Point3D(0,-200,0), Vector3D(0,1,0));
	// plane_ptr->set_material(orange_material);
	// add_object(plane_ptr);
	//
	// Mesh* mesh_ptr = new Mesh();
	// Grid* grid_ptr = new Grid(mesh_ptr);
	//
    // grid_ptr->read_obj_file("./src/cow.obj");       // read obj file
	// grid_ptr->set_material(orange_material);
	//
	// grid_ptr->setup_cells();
	// add_object(grid_ptr);
	// add_mesh(mesh_ptr);
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
