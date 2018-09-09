// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// An OpenCL ray tracer based on Ray Tracing from the Ground Up by Kevin Suffern

__constant float EPSILON = 10e-6f; /* required to compensate for limited float precision */
__constant float PI = 3.14159265359f;

typedef struct Ray{
	double3 o;
	double3 d;
} Ray;

typedef struct Sphere{
	double3 center;
	float3 color;
	double radius;
} Sphere;

typedef struct SceneInfo {
	float3 background_color; // background color of scene
	float s;          // pixel size
	int hres;         // horizontal image resolution
	int vres;         // vertical image resolution
	int num_spheres;  // number of spheres in scene
	int num_samples;  // number of samples per pixel
	int num_sets;     // number of samples patterns
	int seed;         // seed for random number generation
} SceneInfo;

typedef struct ShadeRec{
	double3 normal; //normal at hit point
	double3 local_hit_point; // world coordinates of intersection
	double3 hit_point; // world coordinates of intersection
	float3 color; // color of object hit
	bool hit_an_object; // did the ray hit an object?
} ShadeRec;


void random(int* seed_ptr)
{
	int seed = (*seed_ptr);
	seed = seed ^ (seed << 13);
	seed = seed ^ (seed >> 17);
	seed = seed ^ (seed << 5);
	(*seed_ptr) = seed;
}

bool intersect_sphere(__global Sphere* sphere, const Ray* ray, double* tmin, ShadeRec* sr){
	double t;
	double3 temp = ray->o - sphere->center;
	double a = dot(ray->d, ray->d);
	double b = 2.0 * dot(temp, ray->d);
	double c = dot(temp, temp) - (sphere->radius * sphere->radius);
	double disc = b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			(*tmin) = t;
			sr->normal = (temp + t * ray->d) / sphere->radius;
			sr->local_hit_point = ray->o + t * ray->d;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			(*tmin) = t;
			sr->normal = (temp + t * ray->d) / sphere->radius;
			sr->local_hit_point = ray->o + t * ray->d;
			return (true);
		}
	}

	return (false);
}

ShadeRec hit_bare_bones_objects(const Ray* ray, const int num_spheres, __global Sphere* spheres) {
	ShadeRec sr;
	sr.hit_an_object = false;
	double t;
	double tmin = 10e20f;

	for (int j = 0; j < num_spheres; j++){
		if (intersect_sphere(&spheres[j], ray, &t, &sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.color = spheres[j].color;
		}
	}

	return sr;
}

float3 trace_ray(const Ray* ray, float3 bg_color, const int num_spheres, __global Sphere* spheres) {
	ShadeRec sr = hit_bare_bones_objects(ray, num_spheres, spheres);

	if (sr.hit_an_object)
		return (sr.color);
	else
		return (bg_color);
}

double2 sample_unit_square(__global double2* samples, __global int* shuffled_indices,
	 __private int num_samples, __private int num_sets, __private int* count_ptr,
	 __private int* jump_ptr, __private int* seed_ptr)
{
	random(seed_ptr);

	int random_num = (*seed_ptr);
	int count = (*count_ptr);
	int jump = (*jump_ptr);


	if (count % num_samples == 0){      // start of a new pixel
		jump = (random_num % num_sets) * num_samples;
		(*jump_ptr) = jump;
	}

	(*count_ptr) = count+1;
	return (samples[jump + shuffled_indices[jump + count % num_samples]]);
}

__kernel void tracer(__global float3 *dst,
	__private SceneInfo scene_info, __global double2* samples,
	__global int* shuffled_indices, __global Sphere* spheres)
{
	const int id = get_global_id(0);

	int c = id % scene_info.hres;
	int r = id / scene_info.hres;
	int jump = 0;
	int count = 0;

	Ray ray;
	double zw = 100.0;
	double2 sp; // sample point in [0, 1] x [0, 1]
	double2 pp; // sample point on a pixel
	float3 pixel_color = (float3)(0, 0, 0);

	ray.d = (double3)(0, 0, -1);
	int seed = scene_info.seed;

	for (int j = 0; j < scene_info.num_samples; j++) {
		sp = sample_unit_square(samples, shuffled_indices, scene_info.num_samples, scene_info.num_sets, &count, &jump, &seed);
		pp.s0 = scene_info.s * (c - 0.5 * scene_info.hres + sp.s0);
		pp.s1 = scene_info.s * (r - 0.5 * scene_info.vres + sp.s1);
		ray.o = (double3)(pp.s0, pp.s1, zw);
		pixel_color = pixel_color + trace_ray(&ray, scene_info.background_color, scene_info.num_spheres, spheres);
	}
	pixel_color = pixel_color / scene_info.num_samples;  // average the colors
	dst[id] = pixel_color;
}
