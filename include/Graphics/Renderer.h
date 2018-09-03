#ifndef __RENDERER__
#define __RENDERER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include <string>
#include "RGBColor.h"

class Renderer {
	public:
		virtual ~Renderer();
		virtual void initialize(const int h, const int v) = 0;
		virtual void draw_pixel(const int row,
							const int column,
							const RGBColor& pixel_color) const = 0;
		virtual void display() const = 0;
		virtual void save_bmp(std::string filename) const = 0;
		virtual void save_png(std::string filename) const = 0;
};

#endif