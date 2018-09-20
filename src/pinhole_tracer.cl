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

typedef struct Triangle{
	double3 v0;
	double3 v1;
	double3 v2;
	float3 color;
} Triangle;

typedef struct Plane{
	double3 a;
	double3 n;
	float3 color;
} Plane;

typedef struct Light{
	double3 dir;
	float3 color;
} Light;

typedef struct SceneInfo {
	double3 eye;	  // origin of the camera
	double3 u;        // u vector of camera ONB
	double3 v;        // v vector of camera ONB
	double3 w;        // w vector of camera ONB
	float3 background_color; // background color of scene
	float s;          // pixel size
	float d;          // viewplane distance
	float zoom;       // zoom factor
	float exposure_time; // exposure time
	int hres;         // horizontal image resolution
	int vres;         // vertical image resolution
	int num_planes;  // number of planes in scene
	int num_triangles;  // number of triangles in scene
	int num_spheres;  // number of spheres in scene
	int num_lights;  // number of lights in scene
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

		if (t > EPSILON && t < (*tmin)) {
			(*tmin) = t;
			sr->normal = (temp + t * ray->d) / sphere->radius;
			sr->local_hit_point = ray->o + t * ray->d;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON && t < (*tmin)) {
			(*tmin) = t;
			sr->normal = (temp + t * ray->d) / sphere->radius;
			sr->local_hit_point = ray->o + t * ray->d;
			return (true);
		}
	}

	return (false);
}

bool intersect_plane(__global const Plane* plane, __private const Ray* ray, __private double* tmin, __private ShadeRec* sr){
	float t = dot((plane->a - ray->o), plane->n) / (dot(ray->d, plane->n));

	if (t > EPSILON && t < (*tmin)) {
		(*tmin) = t;
		if(dot(plane->n, ray->d) > 0){
			sr->normal = - plane->n;
		}
		else {
			sr->normal = plane->n;
		}
		sr->local_hit_point = ray->o + t * ray->d;

		return (true);
	}

	return(false);
}

bool intersect_triangle(__global const Triangle* triangle, __private const Ray* ray, __private double* tmin, __private ShadeRec* sr){
	// Find plane intersection
	double3 v10 = triangle->v1 - triangle->v0;
	double3 v20 = triangle->v2 - triangle->v0;
	double3 n = cross(v10, v20); // cross product gives normal of triangle's plane
	double nlen = length(n);
	float t = dot((triangle->v0 - ray->o), n) / (dot(ray->d, n));

	if (t > EPSILON && t < (*tmin)) {
		double3 p = ray->o + t * ray->d;

		double3 A = cross((triangle->v2 - triangle->v1), (p - triangle->v1));
		double3 B = cross(-v20, (p - triangle->v2));
		double3 C = cross(v10, (p - triangle->v0));

		double signA = dot(A, n) > 0 ? 1 : -1; // in triangle?
		double signB = dot(B, n) > 0 ? 1 : -1; // in triangle?
		double signC = dot(C, n) > 0 ? 1 : -1; // in triangle?

		double lambda0 = signA * length(A) / nlen;
		double lambda1 = signB * length(B) / nlen;
		double lambda2 = signC * length(C) / nlen;
		if(
			lambda0 <= 1 && lambda0 >= 0 &&
			lambda1 <= 1 && lambda1 >= 0 &&
			lambda2 <= 1 && lambda2 >= 0
		){
			(*tmin) = t;
			if(dot(n, ray->d) > 0){
				sr->normal = - n;
			}
			else{
				sr->normal = n;
			}
			sr->local_hit_point = p;

			return (true);
		}
	}

	return(false);
}

ShadeRec hit_bare_bones_objects(__private const Ray* ray,
	__private const int num_planes, __global const Plane* planes,
	__private const int num_triangles, __global const Triangle* triangles,
	__private const int num_spheres, __global const Sphere* spheres,
	__private const int num_lights, __global const Light* lights) {
	ShadeRec sr;
	sr.hit_an_object = false;
	double tmin = 10e20f;
	double t = tmin;

	for (int j = 0; j < num_spheres; j++){
		if (intersect_sphere(&spheres[j], ray, &t, &sr)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.color = spheres[j].color;
		}
	}

	for (int j = 0; j < num_triangles; j++){
		if (intersect_triangle(&triangles[j], ray, &t, &sr)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.color = triangles[j].color;
		}
	}

	for (int j = 0; j < num_planes; j++){
		if (intersect_plane(&planes[j], ray, &t, &sr)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.color = planes[j].color;
		}
	}
	//calculate shading
	if(sr.hit_an_object){
		float3 temp = (float3)(0,0,0);
		double kd = 0.8;
		for (int j = 0; j < num_lights; j++){
			Light l = lights[j];
			double3 L = -l.dir;
			L = normalize(L);
			double3 N = sr.normal;
			N = normalize(N);
			float c = kd * max(0.0, dot(L, N));
			temp += c * sr.color;
		}
		temp /= num_lights;
		sr.color = (0.2f * sr.color) + temp;
	}
	return sr;
}

float3 trace_ray(__private const Ray* ray, __private float3 bg_color,
	__private const int num_planes, __global const Plane* planes,
	__private const int num_triangles, __global const Triangle* triangles,
	__private const int num_spheres, __global const Sphere* spheres,
	__private const int num_lights, __global const Light* lights) {
	ShadeRec sr = hit_bare_bones_objects(ray, num_planes, planes, num_triangles,
		triangles, num_spheres, spheres, num_lights, lights );

	if (sr.hit_an_object)
		return (sr.color);
	else
		return (bg_color);
}

double2 sample_unit_square(__global double2* samples, __global int* shuffled_indices,
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

double3 ray_direction(__private double2 pp, __private double3 u,
	__private double3 v, __private double3 w, __private float d)
{
	return (double3)(pp.s0 * u + pp.s1 * v - d * w);
}

__kernel void pinhole_tracer(__global float3 *dst,
	__private SceneInfo scene_info, __global double2* samples,
	__global int* shuffled_indices, __global Plane* planes,
	__global Triangle* triangles, __global Sphere* spheres,
	__global Light* lights)
{
	const int id = get_global_id(0);

	int c = id % scene_info.hres;
	int r = id / scene_info.hres;
	int jump = 0;
	int count = 0;

	Ray ray;
	double2 sp; // sample point in [0, 1] x [0, 1]
	double2 pp; // sample point on a pixel
	float3 pixel_color = (float3)(0, 0, 0);

	float s = scene_info.s / scene_info.zoom;
	ray.o = scene_info.eye;
	ulong seed = 7;

	for (int j = 0; j < scene_info.num_samples; j++) {
		sp = sample_unit_square(samples, shuffled_indices, scene_info.num_samples, scene_info.num_sets, &count, &jump, &seed);
		pp.s0 = s * (c - 0.5 * scene_info.hres + sp.s0);
		pp.s1 = s * (r - 0.5 * scene_info.vres + sp.s1);
		ray.d = ray_direction(pp, scene_info.u, scene_info.v, scene_info.w, scene_info.d);
		pixel_color = pixel_color + trace_ray(&ray, scene_info.background_color,
			scene_info.num_planes, planes, scene_info.num_triangles, triangles,
			scene_info.num_spheres, spheres, scene_info.num_lights, lights);
	}
	pixel_color = pixel_color / scene_info.num_samples * scene_info.exposure_time;;  // average the colors
	dst[id] = pixel_color;
}
