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
#include "SingleSphere.h"
#include "MultipleObjects.h"
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

#include "Pinhole.h"

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
	for(GeometricObject* object: objects){
		delete object;
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
	vp.set_pixel_size(0.5);
	vp.set_sampler(new MultiJittered(25));

	background_color = black;
	tracer_ptr = new MultipleObjects(this);

	Pinhole* pinhole_ptr = new Pinhole();
	pinhole_ptr->set_eye(0, 0, -500);
	pinhole_ptr->set_lookat(0, 0, -50);
	pinhole_ptr->set_view_distance(400); // set d
	pinhole_ptr->set_roll_angle(0); //rotate camera
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	// colors

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

	Point2D sp;           // sample point in [0, 1] x [0, 1]
	Point2D pp;           // sample point on a pixel

	open_window(vp.hres, vp.vres);
	ray.d = Vector3D(0, 0, -1);

	for (int r = 0; r < vp.vres; r++)               // up
		for (int c = 0; c <= vp.hres; c++) {       // across
			pixel_color = black;
			for (int j = 0; j < vp.num_samples; j++) {
				 sp = vp.sampler_ptr->sample_unit_square();
				 pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);
				 pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);
				 ray.o = Point3D(pp.x, pp.y, zw);
				 pixel_color += tracer_ptr->trace_ray(ray);
			}
			pixel_color /= vp.num_samples;  // average the colors
			display_pixel(r, c, pixel_color);
	   }
	renderer->display(); // Display for a second before saving
	renderer->save_png("renders/output.png");
}

void World::opencl_render_scene() const {
	struct CLSceneInfo {
		cl_float3 background_color; // background color of scene
		cl_float s;          // pixel size
		cl_float zw;         // z-position of viewplane
		cl_int hres;         // horizontal image resolution
		cl_int vres;         // vertical image resolution
		cl_int num_spheres;  // number of spheres in scene
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

    std::ifstream t("./src/tracer.cl");
    std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
    // std::cout << str << std::endl;
    const char* source_string = str.c_str();

    // Create an OpenCL program by performing runtime source compilation
    cl::Program program = cl::Program(context, source_string);

    // Build the program and check for compilation errors
    cl_int result = program.build({ device }, "");
    if (result) cout << "Error during compilation! (" << result << ")" << endl;

    // Create a kernel (entry point in the OpenCL source program)
    cl::Kernel kernel = cl::Kernel(program, "tracer");

    // Constructs the arguments for the kernel
	Sampler* sampler = vp.sampler_ptr;

	struct CLSceneInfo cl_info = {
		(cl_float3){background_color.r,background_color.g,background_color.b},
		vp.s,
		75.0,
		vp.hres,
		vp.vres,
		(int) objects.size(),
		vp.num_samples,
		sampler->get_num_sets(),
		(int) time(NULL)
	};
	// std::cout << cl_info.seed <<std::endl;
	// std::cout << cl_info.s <<std::endl;
	// std::cout << cl_info.hres <<std::endl;
	// std::cout << cl_info.vres <<std::endl;
	// std::cout << cl_info.num_spheres <<std::endl;
	// std::cout << cl_info.num_samples <<std::endl;
	// std::cout << cl_info.num_sets <<std::endl;

	int samples_count;
	int indices_count;
	int num_objects = objects.size();
	cl_double2* cl_samples = sampler->get_cl_samples(samples_count);
	cl_int* cl_shuffled_indices = sampler->get_cl_shuffled_indices(indices_count);
	CLSphere* cl_spheres = CLUtil::get_cl_spheres(*this);

    // Create buffers (memory objects) on the OpenCL device, allocate memory and copy input data to device.
    // Flags indicate how the buffer should be used e.g. read-only, write-only, read-write
	cl::Buffer cl_output = cl::Buffer(context, CL_MEM_WRITE_ONLY, vp.hres * vp.vres * sizeof(cl_float3), NULL);
	cl::Buffer cl_buffer_a = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, samples_count * sizeof(cl_double2), cl_samples);
    cl::Buffer cl_buffer_b = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, indices_count * sizeof(cl_int), cl_shuffled_indices);
    cl::Buffer cl_buffer_c = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, num_objects * sizeof(CLSphere), cl_spheres);

    // Specify the arguments for the OpenCL kernel
    kernel.setArg(0, cl_output);
    kernel.setArg(1, cl_info);
    kernel.setArg(2, cl_buffer_a);
    kernel.setArg(3, cl_buffer_b);
    kernel.setArg(4, cl_buffer_c);

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
	open_window(vp.hres, vp.vres);
	int stop = time(NULL) + 10;
	int num_draws = 0;
	SDL_Event e;
	while(time(NULL) < stop){
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
	     	if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP){
				cl_info.zw -= 10;
			    kernel.setArg(1, cl_info);
			}
	     	if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN){
				cl_info.zw += 10;
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

	    renderer->cl_draw(cpu_output);
		num_draws ++;
	}
	cout << num_draws << " draws" << endl;
    renderer->display();
	renderer->save_png("renders/cl_output.png");
}

void World::open_window(const int hres, const int vres) const {
	renderer->initialize(hres, vres);
}

void World::display_pixel(	const int row,
					const int column,
					const RGBColor& color) const {
	renderer->draw_pixel(vp.vres-row, column, color);
}
