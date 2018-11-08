#ifndef __WORLD__
#define __WORLD__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include <vector>

#include "RGBColor.h"
#include "ShadeRec.h"
#include "Ray.h"
#include "Mesh.h"
#include "ViewPlane.h"
#include "Renderer.h"
#include "Tracer.h"
#include "Ambient.h"
#include "Sampler.h"
#include "GeometricObject.h"
class Camera; //fwd declaration

class World {
	public:

		std::vector <GeometricObject*> objects;
		std::vector <Light*> lights;
		std::vector <Mesh*> meshes;
		std::vector <Sampler*> double2_samplers;
		std::vector <Sampler*> double3_samplers;
		ViewPlane       vp;
		RGBColor        background_color;
		Ambient*        ambient_ptr;
		Tracer*         tracer_ptr;
		Renderer*       renderer;
		Camera*         camera_ptr;
		int 			cur_mesh_cl_index;
		int 			cur_d2_samples_cl_index;
		int 			cur_d3_samples_cl_index;

		World();

		~World();

		void close();

		void build();

		void add_object(GeometricObject* object_ptr);

		void add_light(Light* light_ptr);

		void add_mesh(Mesh* mesh_ptr);

		void add_double2_sampler(Sampler* sampler_ptr);

		void add_double3_sampler(Sampler* sampler_ptr);

		ShadeRec hit_bare_bones_objects(const Ray& ray) const;
		ShadeRec hit_objects(const Ray& ray) const;

		void render_scene(void) const;

		void opencl_render_scene() const;

		void open_window(const int hres, const int vres) const;

		void set_camera(Camera* camera);

		void set_ambient_light(Ambient* ambient);

		RGBColor max_to_one(const RGBColor& c) const;

		RGBColor clamp_to_color(const RGBColor& c) const;

		void display_pixel(	const int row,
							const int column,
							const RGBColor& pixel_color) const;
};

#include "Camera.h"
inline void World::add_object(GeometricObject* object_ptr) {
      objects.push_back(object_ptr);
}

inline void World::add_light(Light* light_ptr) {
      lights.push_back(light_ptr);
}

inline void World::add_mesh(Mesh* mesh_ptr) {
	mesh_ptr->set_cl_index(cur_mesh_cl_index);
	cur_mesh_cl_index += mesh_ptr->num_vertices;
    meshes.push_back(mesh_ptr);
}

inline void World::add_double2_sampler(Sampler* sampler_ptr) {
	sampler_ptr->set_samples_index(cur_d2_samples_cl_index);
	sampler_ptr->set_sampler_type((char)2);
	cur_d2_samples_cl_index += sampler_ptr->get_num_samples()*sampler_ptr->get_num_sets();
    double2_samplers.push_back(sampler_ptr);
}

inline void World::add_double3_sampler(Sampler* sampler_ptr) {
	sampler_ptr->set_samples_index(cur_d3_samples_cl_index);
	sampler_ptr->set_sampler_type((char)3);
	cur_d3_samples_cl_index += sampler_ptr->get_num_samples()*sampler_ptr->get_num_sets();
    double3_samplers.push_back(sampler_ptr);
}

inline void World::set_camera(Camera* camera) {
    camera_ptr = camera;
}

inline void World::set_ambient_light(Ambient* ambient) {
    ambient_ptr = ambient;
}

#endif
