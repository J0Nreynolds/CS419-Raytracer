# CS419-Raytracer
A raytracer built during my the Fall 2018 semester of UIUC's CS419: Production Computer Graphics. Built in C/C++, and it may support OpenCL (realtime mobile raytracing?) given time constraints.

## Setup

I followed these instructions [here](https://rageandqq.github.io/blog/2018/03/09/opencl-mac-cpp.html) (seems that MacOS OpenCL headers for C are included, but those for C++ are not) and used [this code sample](from https://gist.github.com/ddemidov/2925717) to get OpenCL working. I had to copy a distribution of the OpenCL C++ headers, `cl.hpp`, to my /System/Library/Frameworks/OpenCL.framework/Headers folder in order to include OpenCL bindings in my project.

I ran into some issues during this, but I was able to get this working after installing a new version of g++ through Homebrew (g++).

I am using SDL for drawing graphics. You can use the steps [here](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/mac/index.php) to setup SDL on MacOS.

I use libpng to convert the SDL texture I render to into a PNG. See `SDLRenderer::save_png(string filename)`. The render texture is drawn to an `SDL_Surface`, where the pixel data can be read and saved.
