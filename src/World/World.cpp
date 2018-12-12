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
#include "EnvironmentLight.h"

#include "Rectangle.h"
#include "AreaLighting.h"

#include "Whitted.h"
#include "Reflective.h"
#include "GlossyReflector.h"

#include "ConstantColor.h"
#include "ImageTexture.h"
#include "SV_Matte.h"
#include "SphericalMap.h"

#include "Transparent.h"

#include "MultipleObjects.h"
#include "RayCast.h"

#include "Instance.h"
#include "LightProbe.h"

#include "Checker3D.h"
#include "WrappedFBmTexture.h"
#include "CubicNoise.h"

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
	int num_samples = 225;

	MultiJittered* sampler_ptr = new MultiJittered(num_samples);

	vp.set_gamma(1.0);
	vp.set_show_out_of_gamut(false);
	vp.set_hres(800);
	vp.set_vres(600);
	vp.set_pixel_size(3);
	vp.set_sampler(sampler_ptr);
	add_double2_sampler(sampler_ptr);

	vp.set_max_depth(9);

	tracer_ptr = new Whitted(this);

	// BEGIN MATERIALS

	Reflective* reflective_ptr = new Reflective();
	reflective_ptr->set_ka(0);
	reflective_ptr->set_kd(0);
	reflective_ptr->set_cd(RGBColor(0, 0, 0));
	reflective_ptr->set_ks(0);
	reflective_ptr->set_exp(100);
	reflective_ptr->set_kr(1);
	reflective_ptr->set_cr(RGBColor(1, 0.7, 0.7));

	float exp = 5000.0;
	RGBColor glossy_color = 0.75*RGBColor(1.0, 215.0/255, 0);
	GlossyReflector* glossy = new GlossyReflector();
	glossy->set_samples(num_samples, exp);
	glossy->set_ka(0.0);
	glossy->set_kd(0.0);
	glossy->set_ks(0.0);
	glossy->set_exp(exp);
	glossy->set_cd(glossy_color);
	glossy->set_kr(0.9);
	glossy->set_exponent(exp);
	glossy->set_cr(glossy_color);

	Transparent* glass_ptr = new Transparent;
	glass_ptr->set_ka(0.0);
	glass_ptr->set_kd(0);
	// glass_ptr->set_ks(0.0);
	glass_ptr->set_exp(2000.0);
	glass_ptr->set_ior(1.5);
	glass_ptr->set_kr(0.1);
	glass_ptr->set_kt(0.9);

	SV_Matte* checker_matte = new SV_Matte;
	checker_matte->set_ka(0.15);
	checker_matte->set_kd(0.85);
	Checker3D* checker_ptr = new Checker3D(white, black, 1.5);
	checker_matte->set_cd(checker_ptr);

	SV_Matte* globe_mat_ptr = new SV_Matte;
	globe_mat_ptr->set_ka(0.15);
	globe_mat_ptr->set_kd(0.85);
	ImageTexture* globe_image = new ImageTexture("./src/world_equirectangular.jpg");
	SphericalMap* globe_spherical_ptr = new SphericalMap();
	globe_image->set_mapping(globe_spherical_ptr);
	globe_mat_ptr->set_cd(globe_image);

	SV_Matte* skybox_matte = new SV_Matte;
	skybox_matte->set_ka(1);
	skybox_matte->set_kd(0.0);
	// ImageTexture* skybox = new ImageTexture("./src/uffizi_probe.hdr");
	// LightProbe* light_probe_ptr = new LightProbe();
	ImageTexture* skybox = new ImageTexture("./src/mountain_eq.jpg");
	SphericalMap* spherical_ptr = new SphericalMap();
	spherical_ptr->set_equirectangular(true);
	skybox->set_mapping(spherical_ptr);
	skybox_matte->set_cd(skybox);

	SV_Matte* marble_matte = new SV_Matte;
	marble_matte->set_ka(0.15);
	marble_matte->set_kd(0.85);
	LatticeNoise* noise = new CubicNoise();
	noise->set_gain(0.5);
	noise->set_lacunarity(6);
	noise->set_num_octaves(6);
	float expansion_number = 3.0;
	WrappedFBmTexture* fbm_ptr = new WrappedFBmTexture(noise, RGBColor(1, 1, 0.38), expansion_number);
	marble_matte->set_cd(fbm_ptr);

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(14.0);
	emissive_ptr->set_ce(RGBColor(0.2, 0.2, 0.9));

	// END MATERIALS

	MultiJittered* occ_sampler_ptr = new MultiJittered(num_samples);

	AmbientOccluder* occluder_ptr = new AmbientOccluder;
	occluder_ptr->set_color(white);
	occluder_ptr->set_ls(2);
	occluder_ptr->set_min_amount(0.15);
	occluder_ptr->set_sampler(occ_sampler_ptr);
	set_ambient_light(occluder_ptr);
	add_double3_sampler(occ_sampler_ptr);

	ThinLens* thinlens_ptr = new ThinLens();
	thinlens_ptr->set_eye(0, 2, 8);
	thinlens_ptr->set_lookat(0, 1, 0);
	thinlens_ptr->set_view_distance(1000); // set d
	thinlens_ptr->set_roll_angle(0); //rotate camera
	thinlens_ptr->set_lens_radius(0.10);
	// thinlens_ptr->set_lens_radius(0);
	thinlens_ptr->set_focal_plane_distance(7);
	thinlens_ptr->compute_uvw();
	thinlens_ptr->set_sampler(new MultiJittered(num_samples));
	set_camera(thinlens_ptr);

	// Pinhole* pinhole_ptr = new Pinhole();
	// pinhole_ptr->set_eye(0, 2, 8);
	// pinhole_ptr->set_lookat(0, 1, 0);
	// pinhole_ptr->set_view_distance(1000); // set d
	// pinhole_ptr->set_roll_angle(0); //rotate camera
	// pinhole_ptr->compute_uvw();
	// set_camera(pinhole_ptr);

	MultiJittered* env_sampler_ptr = new MultiJittered(num_samples);

	Emissive* env_emissive_ptr = new Emissive();
	env_emissive_ptr->set_ce(204.0/255, 220.0/255, 1);
	env_emissive_ptr->scale_radiance(2);

	EnvironmentLight* light_ptr = new EnvironmentLight;
	light_ptr->set_material(env_emissive_ptr);
	light_ptr->set_sampler(env_sampler_ptr);
	light_ptr->set_shadows(true);
	add_light(light_ptr);
	add_double3_sampler(env_sampler_ptr);

	Instance* CS_instance = new Instance();
	Instance* CS419_instance = new Instance();
	Instance* skybox_instance = new Instance();
	Instance* sphere_instance = new Instance();
	Instance* globe_instance = new Instance();
	Instance* teapot_instance = new Instance();
	Instance* cow_instance = new Instance();
	Instance* area_rect_instance = new Instance();

	Grid* grid_ptr = new Grid();

	Plane* plane_ptr1 = new Plane(Point3D(0), Normal(0, 1, 0));
	plane_ptr1->set_material(checker_matte);
	add_object(plane_ptr1);

	// CS MESH
	Mesh* mesh_ptr = new Mesh();
	Grid* CS_ptr = new Grid(mesh_ptr);
	CS_ptr->read_obj_file("./src/CS419-CS.obj", false);       // read obj file
	CS_ptr->setup_cells();
	CS_ptr->set_material(glossy);
	add_mesh(mesh_ptr);

	CS_instance->set_object(CS_ptr);
	CS_instance->translate(Vector3D(0.2, 0.66, 1));
	CS_instance->scale(3.5);
	CS_instance->compute_bounding_box();
	grid_ptr->add_object(CS_instance);

	// 419 MESH
	Mesh* mesh419_ptr = new Mesh();
	Grid* CS419_ptr = new Grid(mesh419_ptr);
	CS419_ptr->read_obj_file("./src/CS419-419.obj", false);       // read obj file
	CS419_ptr->setup_cells();
	CS419_ptr->set_material(glossy);
	add_mesh(mesh419_ptr);

	CS419_instance->set_object(CS419_ptr);
	CS419_instance->translate(Vector3D(0, 1, 0.5));
	CS419_instance->scale(3.5);
	CS419_instance->compute_bounding_box();
	grid_ptr->add_object(CS419_instance);

	// SKYBOX
	Sphere* skybox_sphere = new Sphere (Point3D(0), 1);
	skybox_sphere->set_material(skybox_matte);
	skybox_sphere->set_shadows(false);
	skybox_instance->set_object(skybox_sphere);
	skybox_instance->rotate_y(-70);
	skybox_instance->scale(10000);
	skybox_instance->transform_texture(true);
	add_object(skybox_instance);

	// REFLECTIVE Sphere
	Sphere* sphere = new Sphere (Point3D(0), 1);
	sphere->set_material(reflective_ptr);
	sphere_instance->set_object(sphere);
	sphere_instance->translate(Vector3D(7, 1, 2.5));
	sphere_instance->scale(0.8);
	sphere_instance->compute_bounding_box();
	grid_ptr->add_object(sphere_instance);

	// GLOBE Sphere
	Sphere* globe = new Sphere (Point3D(0), 1);
	globe->set_material(globe_mat_ptr);
	globe_instance->set_object(globe);
	globe_instance->rotate_y(57);
	globe_instance->translate(Vector3D(0.6, 1, 5.5));
	globe_instance->scale(0.85);
	globe_instance->transform_texture(true);
	globe_instance->compute_bounding_box();
	grid_ptr->add_object(globe_instance);

	// TEAPOT marble
	Mesh* teapot_mesh_ptr = new Mesh();
	Grid* teapot_ptr = new Grid(teapot_mesh_ptr);
	teapot_ptr->read_obj_file("./src/teapot.obj", false);       // read obj file
	add_mesh(teapot_mesh_ptr);
	teapot_ptr->set_material(glass_ptr);
	teapot_ptr->setup_cells();
	teapot_instance->set_object(teapot_ptr);
	teapot_instance->rotate_y(-120);
	teapot_instance->translate(Vector3D(-4, 0.5, 12));
	teapot_instance->scale(0.4);
	teapot_instance->compute_bounding_box();
	grid_ptr->add_object(teapot_instance);

	// COW marble
	Mesh* cow_mesh_ptr = new Mesh();
	Grid* cow_ptr = new Grid(cow_mesh_ptr);
	cow_ptr->read_obj_file("./src/cow.obj", false);       // read obj file
	add_mesh(cow_mesh_ptr);
	cow_ptr->set_material(marble_matte);
	cow_ptr->setup_cells();
	cow_instance->set_object(cow_ptr);
	cow_instance->rotate_y(-80);
	cow_instance->translate(Vector3D(1.75, 0.5, 3.9));
	cow_instance->scale(1.25);
	cow_instance->transform_texture(true);
	cow_instance->compute_bounding_box();
	grid_ptr->add_object(cow_instance);

	// AREA LIGHT
	// MultiJittered* area_sampler_ptr = new MultiJittered(num_samples);
	//
	// Rectangle* rectangle_ptr = new Rectangle(Point3D(-7.5, 20, -7.5), Vector3D(15, 0, 0), Vector3D(0, 0, 15));
	// rectangle_ptr->set_material(emissive_ptr);
	// rectangle_ptr->set_shadows(false);
	// rectangle_ptr->set_sampler(area_sampler_ptr);
	// add_double2_sampler(area_sampler_ptr);
	//
	// AreaLight* area_light_ptr = new AreaLight;
	// area_light_ptr->set_object(rectangle_ptr);
	// area_light_ptr->set_shadows(true);
	// add_light(area_light_ptr);
	// area_rect_instance->set_object(rectangle_ptr);

	// grid_ptr->add_object(area_rect_instance);

	// SETUP GRID

	grid_ptr->setup_cells();
	add_object(grid_ptr);

	DirectionalLight* dl = new DirectionalLight(Vector3D(0.25, -0.6, -1));
	dl->set_shadows(true);
	dl->set_ls(1.5);
	add_light(dl);
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
	int y = vp.vres - row  -1;
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
