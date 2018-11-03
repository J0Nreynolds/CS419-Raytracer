#ifndef __CL_UTIL__
#define __CL_UTIL__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "CLSphere.h"
#include "CLTriangle.h"
#include "CLMeshTriangle.h"
#include "CLPlane.h"
#include "World.h"

class CLUtil {
	public:
        static cl::Device choose_platform_and_device();
		template <class To, class From> static void get_all_cl(const World& world, To*& output, int& num_type);
		static void get_cl_spheres(const World& world, CLSphere*& spheres, int& num_spheres);
		static void get_cl_triangles(const World& world, CLTriangle*& triangles, int& num_triangles);
		static void get_cl_mesh_triangles(const World& world, CLMeshTriangle*& mesh_triangles, int& num_mesh_triangles);
		static void get_cl_mesh_vertices(const World& world, cl_double3*& mesh_vertices, int& num_mesh_vertices);
		static void get_cl_mesh_normals(const World& world, cl_double3*& mesh_normals, int& num_mesh_normals);
		static void get_cl_planes(const World& world, CLPlane*& planes, int& num_planes);
		static void get_cl_lights(const World& world, CLLight*& lights, int& num_lights);
		static void attempt_build_program(cl::Program program, cl::Device device);
};


inline template <class To, class From>
void CLUtil::get_all_cl(const World& world, To*& output, int& num_type) {
    num_type = 0;
    int num_objects = world.objects.size();
	for(int i = 0; i < num_objects; i ++){
        From* obj = dynamic_cast<From*> (world.objects[i]);
        if(obj != NULL){
            num_type ++;
        }
	}
    output = new To[num_type];
    int j = 0;
    for(int i = 0; i < num_objects && j < num_type; i++){
        From* obj = dynamic_cast<From*> (world.objects[i]);
        if(obj != NULL){
            output[j++] = obj->get_cl();
        }
    }
}

#endif
