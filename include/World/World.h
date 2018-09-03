#ifndef __WORLD__
#define __WORLD__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "ViewPlane.h"
#include "Renderer.h"
#include "RGBColor.h"
#include "Sphere.h"
#include "Tracer.h"

class World {
	public:

		ViewPlane       vp;
		RGBColor        background_color;
		Sphere          sphere;
		Tracer*         tracer_ptr;
		Renderer*       renderer;

		World(void);

		~World(void);

		void close(void);

		void build(void);

		void render_scene(void) const;

		void open_window(const int hres, const int vres) const;

		void display_pixel(	const int row,
							const int column,
							const RGBColor& pixel_color) const;
};

#endif
