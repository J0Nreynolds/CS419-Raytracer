#ifndef __ORTHOGRAPHIC__
#define __ORTHOGRAPHIC__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Camera.h"

class Orthographic: public Camera {
	public:
		Orthographic();
		Orthographic(Point3D e, Point3D l);
		Orthographic(Point3D e, Point3D l, Vector3D u);
		Orthographic(Point3D e, Point3D l, Vector3D u, float exp);

		virtual void render_scene(World& w);

		virtual void opencl_render_scene(World& w);
};

#endif
