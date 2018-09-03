#ifndef __SDL_RENDERER__
#define __SDL_RENDERER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "SDL.h"

#include "Renderer.h"
#include "World.h"

class SDLRenderer : public Renderer {
	public:
		SDLRenderer();
		~SDLRenderer();
		void initialize(const int h, const int v);
		void draw_pixel(const int row,
							const int column,
							const RGBColor& pixel_color) const;
		void display() const;
		void save_bmp(std::string filename) const;
		void save_png(std::string filename) const;

	protected:
		SDL_Window*     window;
		SDL_Renderer*   renderer;
		SDL_Texture*    texture;
};

#endif
