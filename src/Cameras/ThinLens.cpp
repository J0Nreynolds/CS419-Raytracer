// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class ThinLens
#include "SDL.h"
#include <time.h>
#include <fstream>
#include <iostream>

#include "CLUtil.h"

#include "ThinLens.h"

using namespace std;

ThinLens::ThinLens()
: Camera(), lens_radius(0.0), zoom(1.0), sampler_ptr(NULL) {}

ThinLens::ThinLens(Point3D e, Point3D l)
: Camera(e, l), lens_radius(0.0), zoom(1.0), sampler_ptr(NULL) {}

ThinLens::ThinLens(Point3D e, Point3D l, Vector3D u)
: Camera(e, l, u), lens_radius(0.0), zoom(1.0), sampler_ptr(NULL) {}

ThinLens::ThinLens(Point3D e, Point3D l, Vector3D u, float exp)
: Camera(e, l, u, exp), lens_radius(0.0), zoom(1.0), sampler_ptr(NULL) {}

ThinLens::~ThinLens() {
	if(sampler_ptr != NULL){
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
}

void ThinLens::render_scene(World& w) {
	RGBColor L;
	Ray ray;
	ViewPlane vp(w.vp);

	Point2D sp;               // sample point in [0, 1] X [0, 1]
	Point2D pp;               // sample point on a pixel
	Point2D dp;               // sample point on unit disk
	Point2D lp;               // sample point on lens

	w.open_window(vp.hres, vp.vres);
	vp.s /= zoom;

	for (int r = 0; r < vp.vres; r++)        // up
		for (int c = 0; c < vp.hres; c++) {  // across
			L = black;

			for (int n = 0; n < vp.num_samples; n++) {
				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);
				pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);

				dp = sampler_ptr->sample_unit_disk();
				lp = dp * lens_radius;

				ray.o = eye + lp.x * u + lp.y * v;
				ray.d = ray_direction(pp, lp);
				L += w.tracer_ptr->trace_ray(ray, 0);
			}

			L *= exposure_time / vp.num_samples;
			w.display_pixel(r, c, L);
		}
	w.renderer->display();
	w.renderer->save_png("renders/thin_lens.png");
}

void ThinLens::opencl_render_scene(World& w) {
	struct CLSceneInfo {
		CLLight ambient_light; // ambient light coming from background of scene
		cl_double3 eye; 	// origin of the camera
		cl_double3 u; 		// u vector of camera ONB
		cl_double3 v; 		// v vector of camera ONB
		cl_double3 w; 		// w vector of camera ONB
		cl_float3 background_color; // background color of scene
		cl_float s;          // pixel size
		cl_float radius;     // lens radius
		cl_float d;          // viewplane distance
		cl_float f;          // focal plane distance
		cl_float zoom;       // zoom factor
		cl_float exposure_time; // exposure time
		cl_int hres;         // horizontal image resolution
		cl_int vres;         // vertical image resolution
		cl_int num_planes;  // number of planes in scene
		cl_int num_triangles;  // number of triangles in scene
		cl_int num_spheres;  // number of spheres in scene
		cl_int num_lights;  // number of lights in scene
		cl_int num_samples;  // number of samples per pixel
		cl_int num_sets;     // number of samples patterns
		cl_int seed;         // seed for random num generation
	};
	cl::Device device = CLUtil::choose_platform_and_device();

	// Create an OpenCL context on that device.
	// the context manages all the OpenCL resources
	cl::Context context = cl::Context(device);

	///////////////////
	// OPENCL KERNEL //
	///////////////////

	std::ifstream t("./src/thin_lens_raycast_tracer.cl");
	std::string str((std::istreambuf_iterator<char>(t)),
				  std::istreambuf_iterator<char>());
	const char* source_string = str.c_str();

	// Create an OpenCL program by performing runtime source compilation
	cl::Program program = cl::Program(context, source_string);

	CLUtil::attempt_build_program(program, device);

	// Create a kernel (entry point in the OpenCL source program)
	cl::Kernel kernel = cl::Kernel(program, "thin_lens_tracer");

	ViewPlane vp(w.vp);
	// Constructs the arguments for the kernel
	Sampler* sampler = w.vp.sampler_ptr;

	int samples_count;
	int indices_count;
	int disk_samples_count;
	int disk_indices_count;
	cl_double2* cl_samples = sampler->get_cl_samples(samples_count);
	cl_int* cl_shuffled_indices = sampler->get_cl_shuffled_indices(indices_count);
	cl_double2* cl_disk_samples = sampler_ptr->get_cl_disk_samples(disk_samples_count);
	cl_int* cl_disk_shuffled_indices = sampler_ptr->get_cl_shuffled_indices(disk_indices_count);

	CLPlane* cl_planes;
	int num_planes;
	CLUtil::get_cl_planes(w, cl_planes, num_planes);
	CLTriangle* cl_triangles;
	int num_triangles;
	CLUtil::get_cl_triangles(w, cl_triangles, num_triangles);
	CLSphere* cl_spheres;
	int num_spheres;
	CLUtil::get_cl_spheres(w, cl_spheres, num_spheres);
	CLLight* cl_lights;
	int num_lights;
	CLUtil::get_cl_lights(w, cl_lights, num_lights);


	struct CLSceneInfo cl_info = {
		w.ambient_ptr->get_cl_light(),
		(cl_double3){eye.x, eye.y, eye.z},
		(cl_double3){u.x, u.y, u.z},
		(cl_double3){v.x, v.y, v.z},
		(cl_double3){this->w.x, this->w.y, this->w.z},
		(cl_float3){w.background_color.r, w.background_color.g, w.background_color.b},
		vp.s,
		lens_radius,
		d,
		f,
		zoom,
		exposure_time,
		vp.hres,
		vp.vres,
		num_planes,
		num_triangles,
		num_spheres,
		num_lights,
		vp.num_samples,
		sampler->get_num_sets(),
		(int) time(NULL)
	};

	// Create buffers (memory objects) on the OpenCL device, allocate memory and copy input data to device.
	// Flags indicate how the buffer should be used e.g. read-only, write-only, read-write
	cl::Buffer cl_output = cl::Buffer(context, CL_MEM_WRITE_ONLY, vp.hres * vp.vres * sizeof(cl_float3), NULL);
	cl::Buffer cl_buffer_a = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, samples_count * sizeof(cl_double2), cl_samples);
	cl::Buffer cl_buffer_b = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, indices_count * sizeof(cl_int), cl_shuffled_indices);
	cl::Buffer cl_buffer_c = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, disk_samples_count * sizeof(cl_double2), cl_disk_samples);
	cl::Buffer cl_buffer_d = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, disk_indices_count * sizeof(cl_int), cl_disk_shuffled_indices);
	cl::Buffer cl_buffer_e = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_planes * sizeof(CLPlane), cl_planes);
	cl::Buffer cl_buffer_f = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_triangles * sizeof(CLTriangle), cl_triangles);
	cl::Buffer cl_buffer_g = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_spheres * sizeof(CLSphere), cl_spheres);
	cl::Buffer cl_buffer_h = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_lights * sizeof(CLLight), cl_lights);

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
	int dx = 0;
	int dy = 0;
	int dz = 0;
	while(time(NULL) < stop){
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User input
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q){
				dz = 10;
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e){
				dz = -10;
			}
			if(e.type == SDL_KEYUP && (
				e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_q
			)){
				dz = 0;
			}

			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d){
				dx = 10;
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a){
				dx = -10;
			}
			if(e.type == SDL_KEYUP && (
				e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d
			)){
				dx = 0;
			}

			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w){
				dy = 10;
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s){
				dy = -10;
			}
			if(e.type == SDL_KEYUP && (
				e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_w
			)){
				dy = 0;
			}
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				exit(-1);
			}

			eye = eye + dx * this->u + dy * this->v + dz * this->w;
			cl_info.eye = (cl_double3){eye.x, eye.y, eye.z};
			kernel.setArg(1, cl_info);
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
	w.renderer->save_png("renders/cl_thin_lens.png");
}

Vector3D ThinLens::ray_direction(const Point2D& pixel_point,
	const Point2D& lens_point) const {
	Point2D p;                     // hit point on focal plane
	p.x = pixel_point.x * f / d;
	p.y = pixel_point.y * f / d;

	Vector3D dir = (p.x - lens_point.x) * u + (p.y - lens_point.y) * v - f * w;
	dir.normalize();

	return (dir);
}

void ThinLens::set_sampler(Sampler* sp) {
	if (sampler_ptr != NULL) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}

	sampler_ptr = sp;
	sampler_ptr->map_samples_to_unit_disk();
}
