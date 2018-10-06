// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// An OpenCL ray tracer based on Ray Tracing from the Ground Up by Kevin Suffern

__constant double EPSILON = 10e-6f; /* required to compensate for limited double precision */
__constant double PI = 3.1415926535897932384;
__constant double INV_PI = 0.3183098861837906715;
__constant double kHugeValue = 1.0E20; /* required to compensate for limited double precision */


typedef struct Ray{
	double3 o;
	double3 d;
} Ray;

typedef struct BRDF {
    float3 cd;
    float kd;
    float ks;
    float e;
} BRDF;

typedef struct Material {
    BRDF ambient_brdf;
    BRDF diffuse_brdf;
    BRDF specular_brdf;
} Material;

typedef struct Sphere{
	Material material;
	double3 center;
	double radius;
} Sphere;

typedef struct Triangle{
	Material material;
	double3 v0;
	double3 v1;
	double3 v2;
} Triangle;

typedef struct Plane{
	Material material;
	double3 a;
	double3 n;
} Plane;

typedef struct Light{
	double3 pos;
	double3 dir;
	float3  color;
	float   ls;
	bool    shadows;
} Light;

typedef struct SceneInfo {
	Light ambient_light; // ambient light coming from background of scene
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
	Ray ray;                 // for specular highlights
	double3 normal;          // normal at hit point
	double3 local_hit_point; // untransformed coordinates of intersection
	double3 hit_point;       // world coordinates of intersection
	float3 color;            // color of object hit
	__global const Material* material_ptr;  // material of object hit
	int depth;               // recursion depth
	bool hit_an_object;      // did the ray hit an object?
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

ShadeRec hit_objects(__private const Ray* ray,
	__private const int num_planes, __global const Plane* planes,
	__private const int num_triangles, __global const Triangle* triangles,
	__private const int num_spheres, __global const Sphere* spheres,
	__private const int num_lights, __global const Light* lights) {
	ShadeRec sr;
	sr.hit_an_object = false;
	double tmin = kHugeValue;
	double t = tmin;

	double3 normal;
	double3 local_hit_point;

	for (int j = 0; j < num_spheres; j++){
		if (intersect_sphere(&spheres[j], ray, &t, &sr)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = &(spheres[j].material);
			sr.hit_point = ray->o + t * ray->d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}
	}

	for (int j = 0; j < num_triangles; j++){
		if (intersect_triangle(&triangles[j], ray, &t, &sr)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = &(triangles[j].material);
			sr.hit_point = ray->o + t * ray->d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}
	}

	for (int j = 0; j < num_planes; j++){
		if (intersect_plane(&planes[j], ray, &t, &sr)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = &(planes[j].material);
			sr.hit_point = ray->o + t * ray->d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}
	}

	if (sr.hit_an_object) {
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}

	return (sr);
}

float3 directionalLight_L(__global const Light* light, __private ShadeRec* sr){
    return light->color * light->ls;
}

float3 pointLight_L(__global const Light* light, __private ShadeRec* sr){
    return light->color * light->ls;
}

float3 light_L(__global const Light* light, __private ShadeRec* sr){
    if(all(light->dir == 0)){
        return pointLight_L(light, sr);
    }
    else {
        return directionalLight_L(light, sr);
    }
}

float3 ambient_L(__private const Light* light, __private ShadeRec* sr){
    return light->color * light->ls;
}

double3 directionalLight_get_direction(__global const Light* light, __private ShadeRec* sr){
    return normalize(-light->dir);
}

double3 pointLight_get_direction(__global const Light* light, __private ShadeRec* sr){
    return normalize(light->pos - sr->local_hit_point);
}

double3 get_direction(__global const Light* light, __private ShadeRec* sr){
    if(all(light->dir == 0)){
        return pointLight_get_direction(light, sr);
    }
    else {
        return directionalLight_get_direction(light, sr);
    }

}

float3 lambertian_f(BRDF brdf, __private ShadeRec* sr, double3 wo, double3 wi){
	return brdf.cd * brdf.kd * (float)INV_PI;
}
float3 lambertian_rho(BRDF brdf, __private ShadeRec* sr, double3 wo){
	return brdf.cd * brdf.kd;
}

float3 shade_matte(__private ShadeRec* sr, __global const Light* lights,
        __private const int num_lights, __private const Light* ambient_ptr){
    double3 wo = -sr->ray.d;

	BRDF ambient_brdf = sr->material_ptr->ambient_brdf;
	BRDF diffuse_brdf = sr->material_ptr->diffuse_brdf;

    float3 L = lambertian_rho(ambient_brdf, sr, wo) * ambient_L(ambient_ptr, sr);

    for (int j = 0; j < num_lights; j++){
        double3 wi = get_direction(&lights[j], sr);
        float ndotwi = dot(sr->normal, wi);

        if (ndotwi > 0.0)
            L += lambertian_f(diffuse_brdf, sr, wo, wi) * light_L(&lights[j], sr) * ndotwi;
    }

    return (L);
}

float3 glossy_specular_f(BRDF brdf, __private ShadeRec* sr, double3 wo, double3 wi){
    float3 L = (float3)(0, 0, 0);
    float ndotwi = dot(sr->normal, wi);
    double3 r = -wi + 2.0 * sr->normal * ndotwi;
    float rdotwo = dot(r, wo);

    if (rdotwo > 0.0){
        L.s0 = brdf.ks * pow(rdotwo, brdf.e);
        L.s1 = L.s0;
        L.s2 = L.s0;
    }

    return (L);
}

float3 glossy_specular_rho(BRDF brdf, __private ShadeRec* sr, double3 wo){
    return (float3)(0,0,0);
}

float3 shade_phong(__private ShadeRec* sr, __global const Light* lights,
        __private const int num_lights, __private const Light* ambient_ptr){
    double3 wo = -sr->ray.d;

	BRDF ambient_brdf = sr->material_ptr->ambient_brdf;
	BRDF diffuse_brdf = sr->material_ptr->diffuse_brdf;
	BRDF specular_brdf = sr->material_ptr->specular_brdf;

    float3 L = lambertian_rho(ambient_brdf, sr, wo) * ambient_L(ambient_ptr, sr);

    for (int j = 0; j < num_lights; j++) {
        double3 wi = get_direction(&lights[j], sr);
        float ndotwi = dot(sr->normal, wi);

        if (ndotwi > 0.0)
            L += (lambertian_f(diffuse_brdf, sr, wo, wi) +
                glossy_specular_f(specular_brdf, sr, wo, wi)) * light_L(&lights[j], sr) * ndotwi;
    }

    return (L);
}

float3 shade(__private ShadeRec* sr, __global const Light* lights,
        __private const int num_lights, __private const Light* ambient_ptr){
    if(sr->material_ptr->specular_brdf.ks == 0 && sr->material_ptr->specular_brdf.e == 0){
        return shade_matte(sr, lights, num_lights, ambient_ptr);
    }
    else {
        return shade_phong(sr, lights, num_lights, ambient_ptr);
    }
}

float3 trace_ray(__private const Ray* ray, __private float3 bg_color,
	__private const int num_planes, __global const Plane* planes,
	__private const int num_triangles, __global const Triangle* triangles,
	__private const int num_spheres, __global const Sphere* spheres,
	__private const int num_lights, __global const Light* lights,
    __private const Light* ambient_ptr) {
	ShadeRec sr = hit_objects(ray, num_planes, planes, num_triangles,
		triangles, num_spheres, spheres, num_lights, lights );

    if (sr.hit_an_object) {
		sr.ray = *ray;
        return shade(&sr, lights, num_lights, ambient_ptr);
    }
    else
        return bg_color;
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
	return normalize(pp.s0 * u + pp.s1 * v - d * w);
}

float3 max_to_one(__private const float3 c) {
	float max_value = max(c.s0, max(c.s1, c.s2));

	if (max_value > 1.0)
		return (c / max_value);
	else
		return c;
}

float3 clamp_to_color(__private const float3 raw_color) {
	float3 c = raw_color;

	if (raw_color.s0 > 1.0 || raw_color.s1 > 1.0 || raw_color.s2 > 1.0) {
		c.s0 = 1.0; c.s1 = 0.0; c.s2 = 0.0;
	}

	return c;
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
			scene_info.num_spheres, spheres, scene_info.num_lights, lights, &scene_info.ambient_light);
	}
	pixel_color = pixel_color / scene_info.num_samples * scene_info.exposure_time; // average the colors

	dst[id] = max_to_one(pixel_color);
}
