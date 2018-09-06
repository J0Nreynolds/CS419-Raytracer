// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Renderer
#include <png.h>
#include <algorithm>
#include <iostream>

#include "SDLRenderer.h"

Uint32 last_render = 0;
const Uint32 FPS = 60;

SDLRenderer::SDLRenderer():
	window(NULL), renderer(NULL), texture(NULL)
{
}

/**
 * Initializes the renderer with width hres and height vres
 */
void SDLRenderer::initialize(const int hres, const int vres){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf( "Couldn't initialize SDL: %s\n", SDL_GetError() );
		exit(-1);
	}
	window = SDL_CreateWindow("CS419 Ray Tracer", 0, 0, hres, vres, SDL_WINDOW_OPENGL);
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
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, hres, vres);
	if( texture == NULL )
	{
		printf( "Texture could not be created! SDL Error: %s\n", SDL_GetError() );
		exit(-1);
	}
	SDL_SetRenderTarget( renderer, texture );
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( renderer );
	SDL_SetRenderTarget( renderer, NULL );
}

/**
 * Displays the renderer. If the renderer is already displayed, the latest
 * render texture contents will be displayed in the viewport
 */
void SDLRenderer::display() const{
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( renderer );
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    // SDL_Delay(1000);
}

SDLRenderer::~SDLRenderer(){
	if(texture != NULL){
		// Close and destroy the texture
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
	if(renderer != NULL){
		// Close and destroy the renderer
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if(window != NULL){
		// Close and destroy the window
		SDL_DestroyWindow(window);
		window = NULL;
	}
	// Clean up
	SDL_Quit();
}

/**
 * Draws a single pixel on our render texture
 */
void SDLRenderer::draw_pixel(const int row,
					const int column,
					const RGBColor& color) const{
						//Handle events on queue
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 )
	{
		//User requests quit
		if( e.type == SDL_QUIT )
		{
			exit(-1);
		}
	}
	if(SDL_GetTicks()-last_render > 1000/FPS){
	    //Show rendered to texture
	    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
	    SDL_RenderClear( renderer );
	    SDL_RenderCopy(renderer, texture, NULL, NULL);
		last_render = SDL_GetTicks();
	    SDL_RenderPresent(renderer);
	}
	SDL_SetRenderTarget( renderer, texture );
	SDL_SetRenderDrawColor(renderer, 255*color.r, 255*color.g, 255*color.b, 255);
	SDL_RenderDrawPoint(renderer, column, row);
	SDL_SetRenderTarget( renderer, NULL );
}

/**
 * Saves the render texture contents as a .BMP file
 */
void SDLRenderer::save_bmp(std::string filename) const{
	int hres, vres;
	SDL_GetRendererOutputSize(renderer, &hres, &vres);
	SDL_Surface *sshot = SDL_CreateRGBSurfaceWithFormat(0, hres, vres, 8, SDL_PIXELFORMAT_RGB888);

	SDL_Rect view_rect;
	view_rect.x = 0;
	view_rect.y = 0;
	view_rect.w = hres;
	view_rect.h = vres;

	SDL_SetRenderTarget( renderer, texture );
	SDL_RenderReadPixels(renderer, &view_rect, SDL_PIXELFORMAT_RGB888, sshot->pixels, sshot->pitch);
	SDL_SetRenderTarget( renderer, NULL );
	SDL_SaveBMP(sshot, filename.c_str());
	SDL_FreeSurface(sshot);
}

/**
 * Takes a SDL_Surface and saves its contents as a .PNG file using libpng
 */
void save_surface_to_png(SDL_Surface* surf, std::string filename) {
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep row = NULL;

	fp = fopen(filename.c_str(), "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename.c_str());
	}
	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
	}png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, surf->w, surf->h,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	// 3 bytes = 3 colors * 1 byte (8 bit depth)
	int row_len = 3 * 1 * surf->w;
	row = (png_bytep) malloc(row_len);

	// Write image data
	// Keep in mind that Surface seems to keep pixels in BGRA layout in pixel
	// buffer
	for (int y=0 ; y<surf->h ; y++) {
		for(int x = 0; x < surf->w; x++){
			memcpy(row + x*3, (char *)surf->pixels + y*surf->pitch + x*4, 3);
			// Translate *BGR*A from surface to RGB for PNG
			std::reverse(row+x*3, row+x*3 + 3);
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);
}

/**
 * Saves the render texture contents as a .BMP file
 */
void SDLRenderer::save_png(std::string filename) const{
	int hres, vres;
	SDL_GetRendererOutputSize(renderer, &hres, &vres);
	SDL_Surface *sshot = SDL_CreateRGBSurfaceWithFormat(0, hres, vres, 8, SDL_PIXELFORMAT_RGB888);

	SDL_Rect view_rect;
	view_rect.x = 0;
	view_rect.y = 0;
	view_rect.w = hres;
	view_rect.h = vres;

	SDL_SetRenderTarget( renderer, texture );
	SDL_RenderReadPixels(renderer, &view_rect, SDL_PIXELFORMAT_RGB888, sshot->pixels, sshot->pitch);
	SDL_SetRenderTarget( renderer, NULL );

	save_surface_to_png(sshot, filename.c_str());
	SDL_FreeSurface(sshot);
}
