// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Pinhole
#include "SDL.h"
#include <time.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <iterator>

#include "CLUtil.h"
#include "CLSamplerState.h"
#include "AmbientOccluder.h"

#include "Whitted.h"
#include "Pinhole.h"

using namespace std;

Pinhole::Pinhole()
: Camera(), zoom(1.0) {}

Pinhole::Pinhole(Point3D e, Point3D l)
: Camera(e, l), zoom(1.0) {}

Pinhole::Pinhole(Point3D e, Point3D l, Vector3D u)
: Camera(e, l, u), zoom(1.0) {}

Pinhole::Pinhole(Point3D e, Point3D l, Vector3D u, float exp)
: Camera(e, l, u, exp), zoom(1.0) {}

void Pinhole::render_scene(World& w) {
	RGBColor L;
	ViewPlane vp(w.vp);
	Ray ray;
	int depth = 0;            // recursion depth
	Point2D sp;               // sample point in [0, 1] X [0, 1]
	Point2D pp;               // sample point on a pixel

	w.open_window(vp.hres, vp.vres);
	vp.s /= zoom;
	ray.o = eye;
	using namespace std::chrono;
	int start = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();

	for (int r = 0; r < vp.vres; r++)                // up
		for (int c = 0; c < vp.hres; c++) {    // across
			L = black;
			// std::cout << r << " " <<  c << std::endl;

			for (int j = 0; j < vp.num_samples; j++) {
				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);
				pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);
				ray.d = ray_direction(pp);
				L += w.tracer_ptr->trace_ray(ray, 0);
			}

			L /= vp.num_samples;
			L *= exposure_time;
			w.display_pixel(r, c, L);
		}
	int end = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
	std::cout << (int)(end-start) << "ms to render" << std::endl;
	Whitted* whitted = dynamic_cast<Whitted*>(w.tracer_ptr);
	if(whitted)
		std::cout << whitted->get_ray_count() << " rays used during render" << std::endl;
 	w.renderer->display(); // Display for a second before saving
 	w.renderer->save_png("renders/pinhole.png");
}

void Pinhole::opencl_render_scene(World& w) {
	struct CLSceneInfo {
		CLLight ambient_light; // ambient light coming from background of scene
		cl_double3 eye; 	// origin of the camera
		cl_double3 u; 		// u vector of camera ONB
		cl_double3 v; 		// v vector of camera ONB
		cl_double3 w; 		// w vector of camera ONB
		cl_float3 background_color; // background color of scene
		cl_float s;          // pixel size
		cl_float d;          // viewplane distance
		cl_float zoom;       // zoom factor
		cl_float exposure_time; // exposure time
		cl_int hres;         // horizontal image resolution
		cl_int vres;         // vertical image resolution
		cl_int num_planes;  // number of planes in scene
		cl_int num_triangles;  // number of triangles in scene
		cl_int num_rectangles;  // number of triangles in scene
		cl_int num_spheres;  // number of spheres in scene
		cl_int num_mesh_triangles;  // number of mesh triangles in scene
		cl_int num_lights;  // number of lights in scene
		cl_int num_samplers;  // number of samplers in scene
		cl_int num_samples;  // number of samples per pixel
		cl_int num_sets;     // number of samples patterns
		cl_int vp_sampler_index; // index for viewplane sampler
	};

	cl::Device device = CLUtil::choose_platform_and_device();

    // Create an OpenCL context on that device.
    // the context manages all the OpenCL resources
    cl::Context context = cl::Context(device);

    ///////////////////
    // OPENCL KERNEL //
    ///////////////////

    std::ifstream t("./src/pinhole_area_lighting_tracer.cl");
    std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
    const char* source_string = str.c_str();

    // Create an OpenCL program by performing runtime source compilation
    cl::Program program = cl::Program(context, source_string);

	CLUtil::attempt_build_program(program, device);

    // Create a kernel (entry point in the OpenCL source program)
    cl::Kernel kernel = cl::Kernel(program, "pinhole_tracer");

	ViewPlane vp(w.vp);
    // Constructs the arguments for the kernel
	Sampler* sampler = vp.sampler_ptr;

	CLSampler* cl_samplers;
	int num_samplers;
	CLUtil::get_cl_samplers(w, cl_samplers, num_samplers);

	cl_double2* cl_double2_samples;
	int num_double2_samples;
	CLUtil::get_cl_double2_samples(w, cl_double2_samples, num_double2_samples);

	cl_double3* cl_double3_samples;
	int num_double3_samples;
	CLUtil::get_cl_double3_samples(w, cl_double3_samples, num_double3_samples);

	cl_int* cl_ints;
	int num_ints;
	CLUtil::get_cl_ints(w, cl_ints, num_ints);

	CLPlane* cl_planes;
	int num_planes;
	CLUtil::get_cl_planes(w, cl_planes, num_planes);
	CLTriangle* cl_triangles;
	int num_triangles;
	CLUtil::get_cl_triangles(w, cl_triangles, num_triangles);
	CLRectangle* cl_rectangles;
	int num_rectangles;
	CLUtil::get_cl_rectangles(w, cl_rectangles, num_rectangles);
	CLSphere* cl_spheres;
	int num_spheres;
	CLUtil::get_cl_spheres(w, cl_spheres, num_spheres);

	CLMeshTriangle* cl_mesh_triangles;
	int num_mesh_triangles;
	CLUtil::get_cl_mesh_triangles(w, cl_mesh_triangles, num_mesh_triangles);
	CLLight* cl_lights;
	int num_lights;
	CLUtil::get_cl_lights(w, cl_lights, num_lights);

	cl_double3* cl_mesh_vertices;
	int num_mesh_vertices;
	CLUtil::get_cl_mesh_vertices(w, cl_mesh_vertices, num_mesh_vertices);
	cl_double3* cl_mesh_normals;
	int num_mesh_normals;
	CLUtil::get_cl_mesh_normals(w, cl_mesh_normals, num_mesh_normals);

	struct CLSceneInfo cl_info = {
		w.ambient_ptr->get_cl_light(),
		(cl_double3){eye.x, eye.y, eye.z},
		(cl_double3){u.x, u.y, u.z},
		(cl_double3){v.x, v.y, v.z},
		(cl_double3){this->w.x, this->w.y, this->w.z},
		(cl_float3){w.background_color.r, w.background_color.g, w.background_color.b},
		vp.s,
		d,
		zoom,
		exposure_time,
		vp.hres,
		vp.vres,
		num_planes,
		num_triangles,
		num_rectangles,
		num_spheres,
		num_mesh_triangles,
		num_lights,
		num_samplers,
		vp.num_samples,
		sampler->get_num_sets(),
		sampler->get_cl_index()
	};

    // Create buffers (memory objects) on the OpenCL device, allocate memory and copy input data to device.
    // Flags indicate how the buffer should be used e.g. read-only, write-only, read-write
	cl::Buffer cl_output = cl::Buffer(context, CL_MEM_WRITE_ONLY, vp.hres * vp.vres * sizeof(cl_float3), NULL);
	cl::Buffer cl_buffer_a = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_double2_samples * sizeof(cl_double2), cl_double2_samples);
	cl::Buffer cl_buffer_b = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_double3_samples * sizeof(cl_double3), cl_double3_samples);
	cl::Buffer cl_buffer_c = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_ints * sizeof(cl_int), cl_ints);
	cl::Buffer cl_buffer_d = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_planes * sizeof(CLPlane), cl_planes);
	cl::Buffer cl_buffer_e = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_triangles * sizeof(CLTriangle), cl_triangles);
	cl::Buffer cl_buffer_f = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_rectangles * sizeof(CLRectangle), cl_rectangles);
	cl::Buffer cl_buffer_g = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_spheres * sizeof(CLSphere), cl_spheres);
	cl::Buffer cl_buffer_h = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_mesh_triangles * sizeof(CLMeshTriangle), cl_mesh_triangles);
	cl::Buffer cl_buffer_i = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_lights * sizeof(CLLight), cl_lights);
	cl::Buffer cl_buffer_j = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_samplers * sizeof(CLSampler), cl_samplers);
	cl::Buffer cl_buffer_k = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_mesh_vertices * sizeof(cl_double3), cl_mesh_vertices);
	cl::Buffer cl_buffer_l = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_mesh_normals * sizeof(cl_double3), cl_mesh_normals);
	// cl::Buffer cl_buffer_m = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, num_samplers * sizeof(CLSamplerState), NULL);

    // Specify the arguments for the OpenCL kernel
	kernel.setArg(0, cl_output);
	kernel.setArg(1, cl_info);
	kernel.setArg(2, cl_buffer_a);
	kernel.setArg(3, cl_buffer_b);
	kernel.setArg(4, cl_buffer_c);
	kernel.setArg(5, cl_buffer_d);
	kernel.setArg(6, cl_buffer_e);
	kernel.setArg(7, cl_buffer_f);
	kernel.setArg(8, cl_buffer_g);
	kernel.setArg(9, cl_buffer_h);
	kernel.setArg(10, cl_buffer_i);
	kernel.setArg(11, cl_buffer_j);
	kernel.setArg(12, cl_buffer_k);
	kernel.setArg(13, cl_buffer_l);
	// kernel.setArg(14, cl_buffer_m);

    // Create a command queue for the OpenCL device
    // the command queue allows kernel execution commands to be sent to the device
    cl::CommandQueue queue = cl::CommandQueue(context, device);

    // Determine the global and local number of "work items"
    // The global work size is the total number of work items (threads) that execute in parallel
    // Work items executing together on the same compute unit are grouped into "work groups"
    // The local work size defines the number of work items in each work group
    // Important: global_work_size must be an integer multiple of local_work_size
	std::size_t global_work_size = vp.vres * vp.hres;

	// Open the renderer
	w.open_window(vp.hres, vp.vres);
	int stop = time(NULL) + 10;
	int num_draws = 0;
	SDL_Event e;
	while(time(NULL) < stop){
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User input
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q){
				eye = eye + 10 * this->w;
				cl_info.eye = (cl_double3){eye.x, eye.y, eye.z};
				kernel.setArg(1, cl_info);
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e){
				eye = eye - 10 * this->w;
				cl_info.eye = (cl_double3){eye.x, eye.y, eye.z};
				kernel.setArg(1, cl_info);
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d){
				eye = eye + 10 * this->u;
				cl_info.eye = (cl_double3){eye.x, eye.y, eye.z};
				kernel.setArg(1, cl_info);
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a){
				eye = eye - 10 * this->u;
				cl_info.eye = (cl_double3){eye.x, eye.y, eye.z};
				kernel.setArg(1, cl_info);
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w){
				eye = eye + 10 * this->v;
				cl_info.eye = (cl_double3){eye.x, eye.y, eye.z};
				kernel.setArg(1, cl_info);
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s){
				eye = eye - 10 * this->v;
				cl_info.eye = (cl_double3){eye.x, eye.y, eye.z};
				kernel.setArg(1, cl_info);
			}
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				exit(-1);
			}
		}
	    // Launch the kernel and specify the global and local number of work items (threads)
	    queue.enqueueNDRangeKernel(kernel, 0, global_work_size);

	    // Read and copy OpenCL output to CPU
	    // the "CL_TRUE" flag blocks the read operation until all work items have finished their computation
	    cl_float3* cpu_output = (cl_float3*) queue.enqueueMapBuffer(cl_output, CL_TRUE, CL_MAP_READ, 0, global_work_size * sizeof(cl_float3) );

	    w.renderer->cl_draw(cpu_output);
		num_draws ++;
	}
	cout << num_draws << " draws" << endl;
    w.renderer->display();
	w.renderer->save_png("renders/cl_pinhole.png");
}

Vector3D Pinhole::ray_direction(const Point2D& p) const {
	Vector3D dir = p.x * u + p.y * v - d * w;
	dir.normalize();
	return(dir);
}
