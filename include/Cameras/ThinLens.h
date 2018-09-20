#ifndef __THIN_LENS__
#define __THIN_LENS__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Camera.h"

class ThinLens: public Camera {   public:
	public:
		ThinLens();
		ThinLens(Point3D e, Point3D l);
		ThinLens(Point3D e, Point3D l, Vector3D u);
		ThinLens(Point3D e, Point3D l, Vector3D u, float exp);
		~ThinLens();


		void set_lens_radius(float radius);
		float get_lens_radius();

		void set_focal_plane_distance(float dist);
		float get_focal_plane_distance();

		void set_view_distance(float dist);
		float get_view_distance();

		void set_zoom(float dist);
		float get_zoom();

		void set_sampler(Sampler* sp);

		Vector3D ray_direction(const Point2D& pixel_point, const Point2D& lens_point) const;

		virtual void render_scene(World& w);

		virtual void opencl_render_scene(World& w);

	private:
		float lens_radius;                 // lens radius
		float d;                           // view plane distance
		float f;                           // focal plane distance
		float zoom;                        // zoom factor
		Sampler* sampler_ptr;              // sampler object
};

inline void ThinLens::set_lens_radius(float radius){
	lens_radius = radius;
}

inline float ThinLens::get_lens_radius(){
	return lens_radius;
}

inline void ThinLens::set_view_distance(float dist){
	d = dist;
}

inline float ThinLens::get_view_distance(){
	return d;
}

inline void ThinLens::set_focal_plane_distance(float dist){
	f = dist;
}

inline float ThinLens::get_focal_plane_distance(){
	return f;
}

inline void ThinLens::set_zoom(float z){
	zoom = z;
}

inline float ThinLens::get_zoom(){
	return zoom;
}


#endif
