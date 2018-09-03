// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Renderer
#include "SDLRenderer.h"

SDLRenderer::SDLRenderer():
	hres(0), vres(0), window(NULL), renderer(NULL), texture(NULL)
{
}

void SDLRenderer::initialize(const int h, const int v){
	hres = h;
	vres = v;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf( "Couldn't initialize SDL: %s\n", SDL_GetError() );
		exit(-1);
	}
	window = SDL_CreateWindow("CS419 Ray Tracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, hres, vres, SDL_WINDOW_OPENGL);
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

void SDLRenderer::display() const{
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( renderer );
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
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

void SDLRenderer::draw_pixel(const int row,
					const int column,
					const RGBColor& color) const{
	SDL_SetRenderTarget( renderer, texture );
	SDL_SetRenderDrawColor(renderer, 255*color.r, 255*color.g, 255*color.b, 255);
	SDL_RenderDrawPoint(renderer, hres-column, row);
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
