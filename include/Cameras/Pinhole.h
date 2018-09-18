#ifndef __PINHOLE__
#define __PINHOLE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Camera.h"

class Pinhole: public Camera {
	public:
		Pinhole();
		Pinhole(Point3D e, Point3D l);
		Pinhole(Point3D e, Point3D l, Vector3D u);
		Pinhole(Point3D e, Point3D l, Vector3D u, float exp);

        void set_view_distance(float dist);
        float get_view_distance();

        void set_zoom(float dist);
        float get_zoom();

		Vector3D ray_direction(const Point2D& p) const;

		virtual void render_scene(World& w);

		virtual void opencl_render_scene(World& w);

	private:
		float d;            // view-plane distance
		float zoom;         // zoom factor
};

inline void Pinhole::set_view_distance(float dist){
	d = dist;
}

inline float Pinhole::get_view_distance(){
	return d;
}

inline void Pinhole::set_zoom(float z){
	zoom = z;
}

inline float Pinhole::get_zoom(){
	return zoom;
}

#endif
