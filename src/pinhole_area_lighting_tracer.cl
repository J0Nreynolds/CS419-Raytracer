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

typedef struct Rectangle{
	Material material;
	double3 p0;
	double3 a;
	double3 b;
	double3 normal;
	double a_len_squared;	// square of the length of side a
	double b_len_squared;	// square of the length of side b
	float area;			// for rectangular lights
	float inv_area;		// for rectangular lights
	int sampler_index;	// for rectangular lights
} Rectangle;

typedef struct MeshTriangle{
    Material material;
    double3 normal;
	int mesh_idx;
	int idx0;
	int idx1;
	int idx2;
    char is_smooth;
} MeshTriangle;

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
	int     sampler_index;
	char    shadows;
} Light;

typedef struct Sampler{
	int num_samples;
    int num_sets;
    int samples_index;
    char type;
} Sampler;

typedef struct SamplerState{
    ulong seed;
    int jump;
    int count;
} SamplerState;

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
	int num_rectangles;  // number of rectangles in scene
	int num_spheres;  // number of spheres in scene
	int num_mesh_triangles;  // number of mesh triangles in scene
	int num_lights;  // number of lights in scene
	int num_samplers;  // number of samplers in scene
	int num_samples;  // number of samples per pixel
	int num_sets;     // number of samples patterns
	int vp_sampler_index;  // index for viewplane sampler
} SceneInfo;

typedef struct RenderComponents {
	__global const double2* double2_samples;
	__global const double3* double3_samples;
	__global const int* ints;
	__global const Plane* planes;
	__global const Triangle* triangles;
	__global const Rectangle* rectangles;
	__global const Sphere* spheres;
	__global const MeshTriangle* mesh_triangles;
	__global const Light* lights;
	__global const Sampler* samplers;
	__local SamplerState* sampler_states;
	__global const double3* mesh_vertices;
	__global const double3* mesh_normals;
} RenderComponents;

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


uint random(__local ulong* seed_ptr)
{
	ulong seed = (*seed_ptr);
	seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
	uint result = seed >> 16;
	(*seed_ptr) = result;
	return result;
}

int sample_index(const int sampler_index, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){

	__global const Sampler* sampler = &(render_components->samplers[sampler_index]);
	__local SamplerState* state = &(render_components->sampler_states[sampler_index]);
	__global const int* shuffled_indices = render_components->ints + (sampler_index * sampler->num_sets * sampler->num_samples);

	int random_num = random(&(state->seed));
	int count = (state->count);
	int jump = (state->jump);


	if (count % sampler->num_samples == 0){      // start of a new pixel
		jump = (random_num % sampler->num_sets) * sampler->num_samples;
		state->jump = jump;
	}

	state->count = count+1;
	return (jump + shuffled_indices[jump + (count % sampler->num_samples)]);
}

double2 sample_double2_array(const int sampler_index, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components)
{
	__global const Sampler* sampler = &(render_components->samplers[sampler_index]);
	int index = sample_index(sampler_index, scene_info, render_components);
	return (render_components->double2_samples[sampler->samples_index + index]);
}

double3 sample_double3_array(const int sampler_index, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components)
{
	__global const Sampler* sampler = &(render_components->samplers[sampler_index]);
	int index = sample_index(sampler_index, scene_info, render_components);
	return (render_components->double3_samples[sampler->samples_index + index]);
}

bool intersect_sphere(__global const Sphere* sphere, __private const Ray* ray,
	__private double* tmin, __private ShadeRec* sr){
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

bool shadow_intersect_sphere(__global const Sphere* sphere, __private const Ray* ray,
	__private float* tmin){
	float t;
	double3 temp = ray->o - sphere->center;
	float a = dot(ray->d, ray->d);
	float b = 2.0 * dot(temp, ray->d);
	float c = dot(temp, temp) - (sphere->radius * sphere->radius);
	float disc = b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		float e = sqrt(disc);
		float denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			(*tmin) = t;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			(*tmin) = t;
			return (true);
		}
	}

	return (false);
}

bool intersect_plane(__global const Plane* plane, __private const Ray* ray,
	__private double* tmin, __private ShadeRec* sr){
	double t = dot((plane->a - ray->o), plane->n) / (dot(ray->d, plane->n));

	if (t > EPSILON) {
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

bool shadow_intersect_plane(__global const Plane* plane,__private const Ray* ray,
	__private float* tmin){
	float t = dot((plane->a - ray->o), plane->n) / (dot(ray->d, plane->n));

	if (t > EPSILON) {
		(*tmin) = t;
		return (true);
	}

	return(false);
}

bool intersect_triangle(__global const Triangle* triangle,
	__private const Ray* ray, __private double* tmin, __private ShadeRec* sr){
	// Find plane intersection
	double3 v10 = triangle->v1 - triangle->v0;
	double3 v20 = triangle->v2 - triangle->v0;
	double3 n = cross(v10, v20); // cross product gives normal of triangle's plane
	double nlen = length(n);
	n = normalize(n);
	double t = dot((triangle->v0 - ray->o), n) / (dot(ray->d, n));

	if (t > EPSILON) {
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

bool shadow_intersect_triangle(__global const Triangle* triangle,
	__private const Ray* ray, __private float* tmin){
	// Find plane intersection
	double3 v10 = triangle->v1 - triangle->v0;
	double3 v20 = triangle->v2 - triangle->v0;
	double3 n = cross(v10, v20); // cross product gives normal of triangle's plane
	float nlen = length(n);
	float t = dot((triangle->v0 - ray->o), n) / (dot(ray->d, n));

	if (t > EPSILON) {
		double3 p = ray->o + t * ray->d;

		double3 A = cross((triangle->v2 - triangle->v1), (p - triangle->v1));
		double3 B = cross(-v20, (p - triangle->v2));
		double3 C = cross(v10, (p - triangle->v0));

		float signA = dot(A, n) > 0 ? 1 : -1; // in triangle?
		float signB = dot(B, n) > 0 ? 1 : -1; // in triangle?
		float signC = dot(C, n) > 0 ? 1 : -1; // in triangle?

		float lambda0 = signA * length(A) / nlen;
		float lambda1 = signB * length(B) / nlen;
		float lambda2 = signC * length(C) / nlen;
		if(
			lambda0 <= 1 && lambda0 >= 0 &&
			lambda1 <= 1 && lambda1 >= 0 &&
			lambda2 <= 1 && lambda2 >= 0
		){
			(*tmin) = t;
			return (true);
		}
	}

	return(false);
}

bool intersect_mesh_triangle(__global const MeshTriangle* mesh_triangle,
	__private const Ray* ray, __private double* tmin, __private ShadeRec* sr,
	__global const double3* mesh_vertices, __global const double3* mesh_normals){
	int mesh_idx = mesh_triangle->mesh_idx;
	int idx0 = mesh_triangle->idx0;
	int idx1 = mesh_triangle->idx1;
	int idx2 = mesh_triangle->idx2;

	double3 v0 = mesh_vertices[mesh_idx + idx0];
    double3 v1 = mesh_vertices[mesh_idx + idx1];
    double3 v2 = mesh_vertices[mesh_idx + idx2];

    double a = v0.x - v1.x, b = v0.x - v2.x, c = ray->d.x, d = v0.x - ray->o.x;
    double e = v0.y - v1.y, f = v0.y - v2.y, g = ray->d.y, h = v0.y - ray->o.y;
    double i = v0.z - v1.z, j = v0.z - v2.z, k = ray->d.z, l = v0.z - ray->o.z;

    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, r = e * l - h * i, s = e * j - f * i;

    double inv_denom = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    double beta = e1 * inv_denom;

    if (beta < 0.0 || beta > 1.0)
      return (false);

    double e2 = a * n + d * q + c * r;
    double gamma = e2 * inv_denom;

    if (gamma < 0.0 || gamma > 1.0)
      return (false);

    if (beta + gamma > 1.0)
      return (false);

    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < EPSILON)
      return (false);

	(*tmin) = t;
	if(mesh_triangle->is_smooth){
		double3 n0 = mesh_normals[mesh_idx + idx0];
		double3 n1 = mesh_normals[mesh_idx + idx1];
		double3 n2 = mesh_normals[mesh_idx + idx2];
		sr->normal =  (1 - beta - gamma) * n0 + beta * n1 + gamma * n2;
	}
	else {
		sr->normal = mesh_triangle->normal;                      // for flat shading
	    if(dot(sr->normal, ray->d) > 0){
	        sr->normal = -sr->normal;
	    }
	}
    sr->local_hit_point = ray->o + t * ray->d;  // for texture mapping

    return(true);
}

bool shadow_intersect_mesh_triangle(__global const MeshTriangle* mesh_triangle,
	__private const Ray* ray, __private float* tmin, __global const double3* mesh_vertices){
	int mesh_idx = mesh_triangle->mesh_idx;
	int idx0 = mesh_triangle->idx0;
	int idx1 = mesh_triangle->idx1;
	int idx2 = mesh_triangle->idx2;

	double3 v0 = mesh_vertices[mesh_idx + idx0];
	double3 v1 = mesh_vertices[mesh_idx + idx1];
	double3 v2 = mesh_vertices[mesh_idx + idx2];

	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray->d.x, d = v0.x - ray->o.x;
    double e = v0.y - v1.y, f = v0.y - v2.y, g = ray->d.y, h = v0.y - ray->o.y;
    double i = v0.z - v1.z, j = v0.z - v2.z, k = ray->d.z, l = v0.z - ray->o.z;

    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, r = e * l - h * i, s = e * j - f * i;

    double inv_denom = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    double beta = e1 * inv_denom;

    if (beta < 0.0 || beta > 1.0)
      return (false);

    double e2 = a * n + d * q + c * r;
    double gamma = e2 * inv_denom;

    if (gamma < 0.0 || gamma > 1.0)
      return (false);

    if (beta + gamma > 1.0)
      return (false);

    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < EPSILON)
      return (false);

  	(*tmin) = t;
    return true;
}

ShadeRec hit_objects(__private const Ray* ray, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
	__global const Plane* planes = render_components->planes;
	__global const Triangle* triangles = render_components->triangles;
	__global const Sphere* spheres = render_components->spheres;
	__global const MeshTriangle* mesh_triangles = render_components->mesh_triangles;
	__global const double3* mesh_vertices = render_components->mesh_vertices;
	__global const double3* mesh_normals = render_components->mesh_normals;

	ShadeRec sr;
	sr.hit_an_object = false;
	double tmin = kHugeValue;
	double t = tmin;

	double3 normal;
	double3 local_hit_point;

	for (int j = 0; j < scene_info->num_spheres; j++){
		if (intersect_sphere(&spheres[j], ray, &t, &sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = &(spheres[j].material);
			sr.hit_point = ray->o + t * ray->d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}
	}

	for (int j = 0; j < scene_info->num_triangles; j++){
		if (intersect_triangle(&triangles[j], ray, &t, &sr) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = &(triangles[j].material);
			sr.hit_point = ray->o + t * ray->d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}
	}

	for (int j = 0; j < scene_info->num_mesh_triangles; j++){
		if (intersect_mesh_triangle(&mesh_triangles[j], ray, &t, &sr, mesh_vertices, mesh_normals) && (t < tmin)) {
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = &(mesh_triangles[j].material);
			sr.hit_point = ray->o + t * ray->d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}
	}

	for (int j = 0; j < scene_info->num_planes; j++){
		if (intersect_plane(&planes[j], ray, &t, &sr) && (t < tmin)) {
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

float3 Light_L(__global const Light* light, __private ShadeRec* sr){
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

double3 Light_get_direction(__global const Light* light, __private ShadeRec* sr){
    if(all(light->dir == 0)){
        return pointLight_get_direction(light, sr);
    }
    else {
        return directionalLight_get_direction(light, sr);
    }

}

bool pointLight_in_shadow(__global const Light* light, __private const Ray* ray,
	__private ShadeRec* sr, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
	__global const Plane* planes = render_components->planes;
	__global const Triangle* triangles = render_components->triangles;
	__global const Sphere* spheres = render_components->spheres;
	__global const MeshTriangle* mesh_triangles = render_components->mesh_triangles;
	__global const double3* mesh_vertices = render_components->mesh_vertices;

    float t;
	float d = length(light->pos - ray->o);

	for (int j = 0; j < scene_info->num_planes; j++)
		if (shadow_intersect_plane(&planes[j], ray, &t) && length(t * ray->d) < d)
			return (true);

	for (int j = 0; j < scene_info->num_triangles; j++)
		if (shadow_intersect_triangle(&triangles[j], ray, &t) && length(t * ray->d) < d)
			return (true);

	for (int j = 0; j < scene_info->num_mesh_triangles; j++)
		if (shadow_intersect_mesh_triangle(&mesh_triangles[j], ray, &t, mesh_vertices) && length(t * ray->d) < d)
			return (true);

	for (int j = 0; j < scene_info->num_spheres; j++)
		if (shadow_intersect_sphere(&spheres[j], ray, &t) &&  length(t * ray->d) < d)
			return (true);

	return (false);
}

bool directionalLight_in_shadow(__global const Light* light, __private const Ray* ray,
	__private ShadeRec* sr, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
	__global const Plane* planes = render_components->planes;
	__global const Triangle* triangles = render_components->triangles;
	__global const Sphere* spheres = render_components->spheres;
	__global const MeshTriangle* mesh_triangles = render_components->mesh_triangles;
	__global const double3* mesh_vertices = render_components->mesh_vertices;

	float t;

	for (int j = 0; j < scene_info->num_planes; j++)
		if (shadow_intersect_plane(&planes[j], ray, &t))
			return (true);

	for (int j = 0; j < scene_info->num_triangles; j++)
		if (shadow_intersect_triangle(&triangles[j], ray, &t))
			return (true);

	for (int j = 0; j < scene_info->num_mesh_triangles; j++)
		if (shadow_intersect_mesh_triangle(&mesh_triangles[j], ray, &t, mesh_vertices))
			return (true);

	for (int j = 0; j < scene_info->num_spheres; j++)
		if (shadow_intersect_sphere(&spheres[j], ray, &t))
			return (true);

	return false;
}

bool Light_in_shadow(__global const Light* light, __private const Ray* ray,
	__private ShadeRec* sr, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
    if(all(light->dir == 0)){
        return pointLight_in_shadow(light, ray, sr, scene_info, render_components);
    }
    else {
        return directionalLight_in_shadow(light, ray, sr, scene_info, render_components);
    }
}


double3 ambientOccluder_get_direction(__private const Light* light, __private ShadeRec* sr,
	double3 u, double3 v, double3 w, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
    double3 sp = sample_double3_array(light->sampler_index, scene_info, render_components);
    return (sp.x * u + sp.y * v + sp.z * w);
}

float3 ambientOccluder_L(__private const Light* light, __private ShadeRec* sr,
	__private const SceneInfo* scene_info, __private const RenderComponents* render_components){
    double3 w = sr->normal;
    // jitter up vector in case normal is vertical
    double3 v = cross(w, (double3)(0.0072, 1.0, 0.0034));
    v = normalize(v);
    double3 u = cross(v, w);

    Ray shadow_ray;
    shadow_ray.o = sr->hit_point;
    shadow_ray.d = ambientOccluder_get_direction(light, sr, u, v, w, scene_info, render_components);

    if (directionalLight_in_shadow(0, &shadow_ray, sr, scene_info, render_components)){
		float3 min_amount = (float3)(light->dir.x, light->dir.y, light->dir.z);
        return (min_amount * light->ls * light->color);
	}
    else
        return (light->ls * light->color);
}

float3 lambertian_f(BRDF brdf, __private ShadeRec* sr, double3 wo, double3 wi){
	return brdf.cd * brdf.kd * (float)INV_PI;
}
float3 lambertian_rho(BRDF brdf, __private ShadeRec* sr, double3 wo){
	return brdf.cd * brdf.kd;
}

float3 shade_matte(__private ShadeRec* sr, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
	__global const Light* lights = render_components->lights;

    double3 wo = -sr->ray.d;

	BRDF ambient_brdf = sr->material_ptr->ambient_brdf;
	BRDF diffuse_brdf = sr->material_ptr->diffuse_brdf;

	__private const Light* ambient_light = &(scene_info->ambient_light);
	float3 L;
	if(ambient_light->shadows){ //Render with ambient occlusion
    	L = lambertian_rho(ambient_brdf, sr, wo) * ambientOccluder_L(ambient_light, sr, scene_info, render_components);
	}
	else {
    	L = lambertian_rho(ambient_brdf, sr, wo) * ambient_L(ambient_light, sr);
	}

    for (int j = 0; j < scene_info->num_lights; j++){
        double3 wi = Light_get_direction(&lights[j], sr);
        float ndotwi = dot(sr->normal, wi);

        if (ndotwi > 0.0){
			bool in_shadow = false;

			if (lights[j].shadows) {
				Ray shadow_ray = {sr->hit_point, wi};
				in_shadow = Light_in_shadow(&lights[j], &shadow_ray, sr,
					scene_info, render_components);
			}

			if (!in_shadow)
				L += lambertian_f(diffuse_brdf, sr, wo, wi) * Light_L(&lights[j], sr) * ndotwi;
		}
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

float3 shade_phong(__private ShadeRec* sr, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
	__global const Light* lights = render_components->lights;

    double3 wo = -sr->ray.d;

	BRDF ambient_brdf = sr->material_ptr->ambient_brdf;
	BRDF diffuse_brdf = sr->material_ptr->diffuse_brdf;
	BRDF specular_brdf = sr->material_ptr->specular_brdf;

	__private const Light* ambient_light = &(scene_info->ambient_light);
	float3 L;
	if(ambient_light->shadows){ //Render with ambient occlusion
		L = lambertian_rho(ambient_brdf, sr, wo) * ambientOccluder_L(ambient_light, sr, scene_info, render_components);
	}
	else {
		L = lambertian_rho(ambient_brdf, sr, wo) * ambient_L(ambient_light, sr);
	}

    for (int j = 0; j < scene_info->num_lights; j++) {
        double3 wi = Light_get_direction(&lights[j], sr);
        float ndotwi = dot(sr->normal, wi);

        if (ndotwi > 0.0){
			bool in_shadow = false;

			if (lights[j].shadows) {
				Ray shadow_ray = {sr->hit_point, wi};
				in_shadow = Light_in_shadow(&lights[j], &shadow_ray, sr, scene_info, render_components);
			}

			if (!in_shadow)
				L += (lambertian_f(diffuse_brdf, sr, wo, wi) +
	                glossy_specular_f(specular_brdf, sr, wo, wi)) * Light_L(&lights[j], sr) * ndotwi;
		}
    }

    return (L);
}

float3 shade(__private ShadeRec* sr, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components){
    if(sr->material_ptr->specular_brdf.ks == 0 && sr->material_ptr->specular_brdf.e == 0){
        return shade_matte(sr, scene_info, render_components);
    }
    else {
        return shade_phong(sr, scene_info, render_components);
    }
}

float3 trace_ray(__private const Ray* ray, __private const SceneInfo* scene_info,
	__private const RenderComponents* render_components) {
	ShadeRec sr = hit_objects(ray, scene_info, render_components);

    if (sr.hit_an_object) {
		sr.ray = *ray;
        return shade(&sr, scene_info, render_components);
    }
    else
        return scene_info->background_color;
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
	__private SceneInfo scene_info, __global double2* double2_samples,
	__global double3* double3_samples, __global int* ints,
	__global Plane* planes, __global Triangle* triangles,
	__global Rectangle* rectangles, __global Sphere* spheres,
	__global MeshTriangle* mesh_triangles, __global Light* lights,
	__global Sampler* samplers, __global double3* mesh_vertices,
	__global double3* mesh_normals, __local SamplerState* sampler_states)
{

	const int id = get_global_id(0);

	int c = id % scene_info.hres;
	int r = id / scene_info.hres;

	Ray ray;
	double2 sp; // sample point in [0, 1] x [0, 1]
	double2 pp; // sample point on a pixel
	float3 pixel_color = (float3)(0, 0, 0);

	float s = scene_info.s / scene_info.zoom;
	ray.o = scene_info.eye;

	RenderComponents render_components;
	render_components.double2_samples = double2_samples;
	render_components.double3_samples = double3_samples;
	render_components.ints = ints;
	render_components.planes = planes;
	render_components.triangles = triangles;
	render_components.rectangles = rectangles;
	render_components.spheres = spheres;
	render_components.mesh_triangles = mesh_triangles;
	render_components.lights = lights;
	render_components.samplers = samplers;
	render_components.sampler_states = sampler_states;
	render_components.mesh_vertices = mesh_vertices;
	render_components.mesh_normals = mesh_normals;

	// initialize sampler states
	for (int j = 0; j < scene_info.num_samplers; j++) {
		sampler_states[j].seed = (ulong) c;
		sampler_states[j].count = 0;
		sampler_states[j].jump = 0;
	}

	for (int j = 0; j < scene_info.num_samples; j++) {
		sp = sample_double2_array(scene_info.vp_sampler_index, &scene_info, &render_components);
		pp.s0 = s * (c - 0.5 * scene_info.hres + sp.s0);
		pp.s1 = s * (r - 0.5 * scene_info.vres + sp.s1);
		ray.d = ray_direction(pp, scene_info.u, scene_info.v, scene_info.w, scene_info.d);
		pixel_color += trace_ray(&ray, &scene_info, &render_components);
	}
	pixel_color *= scene_info.exposure_time / scene_info.num_samples ; // average the colors

	dst[id] = max_to_one(pixel_color);
}
