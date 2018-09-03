// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class World
#include <algorithm>
#include <iostream>

#include "World.h"
#include "SingleSphere.h"


World::World(void){
}

World::~World(void){
	// Close and destroy the renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	// Close and destroy the window
	SDL_DestroyWindow(window);
	window = NULL;
	// Close and destroy the texture
	SDL_DestroyTexture(texture);
	texture = NULL;
	// Clean up
	SDL_Quit();
	delete tracer_ptr;
	tracer_ptr = NULL;
}


void World::build(void){
	vp.set_hres(200);
	vp.set_vres(200);
	vp.set_pixel_size(1.0);
	vp.set_gamma(1.0);

	background_color = black;
	tracer_ptr = new SingleSphere(this);

	sphere.set_center(0.0);
	sphere.set_radius(85.0);

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("CS419 Ray Tracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200, SDL_WINDOW_OPENGL);
	if( window == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		exit(-1);
	}
		//Create renderer for window
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if( renderer == NULL )
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		exit(-1);
	}
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 200, 200);
}

void World::render_scene(void) const {
	RGBColor pixel_color;
	Ray ray;
	double zw = 100.0; // hard wired in
	double x, y;

	open_window(vp.hres, vp.vres);

	ray.d = Vector3D(0, 0, -1);

	for (int r = 0; r < vp.vres; r++)               // up
		for (int c = 0; c <= vp.hres; c++) {        // across
			x = vp.s * (c - 0.5 * (vp.hres - 1.0));
			y = vp.s * (r - 0.5 * (vp.vres - 1.0));
			ray.o = Point3D(x, y, zw);
			pixel_color = tracer_ptr->trace_ray(ray);
			display_pixel(r, c, pixel_color);
		}

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);  // Pause execution for 5000 milliseconds, for example

	// Close and destroy the renderer
	SDL_DestroyRenderer(renderer);
	// Close and destroy the window
	SDL_DestroyWindow(window);
	// Clean up
	SDL_Quit();
}

void World::open_window(const int hres, const int vres) const {
    SDL_SetRenderTarget( renderer, texture );
   	//Initialize renderer color
   	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( renderer );

    //Reset render target
    SDL_SetRenderTarget( renderer, NULL );
    //Show rendered to texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);

}

void World::display_pixel(	const int row,
					const int column,
					const RGBColor& color) const {
	SDL_SetRenderTarget( renderer, texture );
	SDL_SetRenderDrawColor(renderer, 255*color.r, 255*color.g, 255*color.b, 255);
	SDL_RenderDrawPoint(renderer, column, vp.vres-row);
	SDL_SetRenderTarget( renderer, NULL );
	if(column % 199 == 0){
	    //Show rendered to texture
	   	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
	    SDL_RenderClear( renderer );
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
}
