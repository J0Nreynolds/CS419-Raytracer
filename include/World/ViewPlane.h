#ifndef __VIEW_PLANE__
#define __VIEW_PLANE__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include <string>

#include "Sampler.h"

class ViewPlane {
	public:

		int         hres;         // horizontal image resolution
		int         vres;         // vertical image resolution
		int         num_samples;  // number of samples per pixel
		float       s;            // pixel size
		float       gamma;        // monitor gamma factor
		float       inv_gamma;    // one over gamma
		Sampler*    sampler_ptr;  // Sampler used for rendering

		void set_hres(const int h_res);
		void set_vres(const int v_res);
		void set_pixel_size(const double size);
		void set_gamma(const double g);
		
		void set_sampler(Sampler* sp);
		void set_samples(const int n);
};

inline void ViewPlane::set_hres(const int h_res) {
	hres = h_res;
}

inline void ViewPlane::set_vres(const int v_res) {
	vres = v_res;
}

inline void ViewPlane::set_pixel_size(const double size) {
	s = size;
}

inline void ViewPlane::set_gamma(const double g) {
	gamma = g;
	inv_gamma = 1.0 / gamma;
}


#endif
