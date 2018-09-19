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
	double3 eye;	  // origin of the camera
	double3 u;        // u vector of camera ONB
	double3 v;        // v vector of camera ONB
	double3 w;        // w vector of camera ONB
	float3 background_color; // background color of scene
	float s;          // pixel size
	float radius;     // lens radius
	float d;          // viewplane distance
	float f;          // focal plane distance
	float zoom;       // zoom factor
	float exposure_time; // exposure time
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


uint random(__private ulong* seed_ptr)
{
	ulong seed = (*seed_ptr);
	seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
	uint result = seed >> 16;
	(*seed_ptr) = result;
	return result;
}

bool intersect_sphere(__global const Sphere* sphere, __private const Ray* ray, __private double* tmin, __private ShadeRec* sr){
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

ShadeRec hit_bare_bones_objects(__private const Ray* ray, __private const int num_spheres, __global const Sphere* spheres) {
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

float3 trace_ray(__private const Ray* ray, __private float3 bg_color, __private const int num_spheres, __global const Sphere* spheres) {
	ShadeRec sr = hit_bare_bones_objects(ray, num_spheres, spheres);

	if (sr.hit_an_object)
		return (sr.color);
	else
		return (bg_color);
}

double3 ray_direction(__private double2 pp, __private double2 lp, __private double3 u,
	__private double3 v, __private double3 w, __private float f, __private float d)
{
	double2 p = pp * f / d;  // hit point on focal plane

	double3 dir = (p.s0 - lp.s0) * u + (p.s1 - lp.s1) * v - f * w;
	return normalize(dir);
}

double2 sample_double2_array(__global double2* samples, __global int* shuffled_indices,
	 __private int num_samples, __private int num_sets, __private int* count_ptr,
	 __private int* jump_ptr, __private ulong* seed_ptr)
{
	int random_num = random(seed_ptr);
	int count = (*count_ptr);
	int jump = (*jump_ptr);


	if (count % num_samples == 0){      // start of a new pixel
		jump = (random_num % num_sets) * num_samples;
		(*jump_ptr) = jump;
	}

	(*count_ptr) = count+1;
	return (samples[jump + shuffled_indices[jump + (count % num_samples)]]);
}

__kernel void thin_lens_tracer(__global float3 *dst,
	__private SceneInfo scene_info, __global double2* samples,
	__global int* shuffled_indices, __global double2* disc_samples,
	__global int* disc_shuffled_indices, __global Sphere* spheres)
{
	const int id = get_global_id(0);

	int c = id % scene_info.hres;
	int r = id / scene_info.hres;
	int jump1 = 0;
	int jump2 = jump1;
	int count1 = 0;
	int count2 = count1;

	Ray ray;
	double2 sp; // sample point in [0, 1] x [0, 1]
	double2 pp; // sample point on a pixel
	double2 dp; // sample point on unit disk
	double2 lp; // sample point on lens

	float3 pixel_color = (float3)(0, 0, 0);

	float s = scene_info.s / scene_info.zoom;
	ulong seed1 = 7;
	ulong seed2 = seed1;

	for (int j = 0; j < scene_info.num_samples; j++) {
		sp = sample_double2_array(samples, shuffled_indices, scene_info.num_samples, scene_info.num_sets, &count1, &jump1, &seed1);
		pp.s0 = s * (c - 0.5 * scene_info.hres + sp.s0);
		pp.s1 = s * (r - 0.5 * scene_info.vres + sp.s1);

		dp = sample_double2_array(disc_samples, disc_shuffled_indices, scene_info.num_samples, scene_info.num_sets, &count2, &jump2, &seed2);
		lp = dp * scene_info.radius;

		ray.o = scene_info.eye + lp.s0 * scene_info.u + lp.s1 * scene_info.v;
		ray.d = ray_direction(pp, lp, scene_info.u, scene_info.v, scene_info.w, scene_info.f, scene_info.d);
		pixel_color = pixel_color + trace_ray(&ray, scene_info.background_color, scene_info.num_spheres, spheres);
	}
	pixel_color = pixel_color / scene_info.num_samples * scene_info.exposure_time;  // average the colors
	dst[id] = pixel_color;
}
