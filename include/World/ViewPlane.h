#ifndef __VIEW_PLANE__
#define __VIEW_PLANE__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include <string>

#include "Sampler.h"

class ViewPlane {
	public:
		bool        show_out_of_gamut = false; // whether to show out-of-gamut colors
		int         hres;         // horizontal image resolution
		int         vres;         // vertical image resolution
		int         num_samples;  // number of samples per pixel
		int         max_depth;    // max recursion depth for reflected rays
		float       s;            // pixel size
		float       gamma;        // monitor gamma factor
		float       inv_gamma;    // one over gamma
		Sampler*    sampler_ptr;  // Sampler used for rendering

		void set_hres(const int h_res);
		void set_vres(const int v_res);
		void set_max_depth(const int depth);
		void set_pixel_size(const double size);
		void set_gamma(const double g);
		void set_show_out_of_gamut(const bool enabled);

		void set_sampler(Sampler* sp);
		void set_samples(const int n);
};

inline void ViewPlane::set_hres(const int h_res) {
	hres = h_res;
}

inline void ViewPlane::set_vres(const int v_res) {
	vres = v_res;
}

inline void ViewPlane::set_max_depth(const int depth) {
	max_depth = depth;
}

inline void ViewPlane::set_pixel_size(const double size) {
	s = size;
}

inline void ViewPlane::set_gamma(const double g) {
	gamma = g;
	inv_gamma = 1.0 / gamma;
}

inline void ViewPlane::set_show_out_of_gamut(const bool enabled) {
	show_out_of_gamut = enabled;
}


#endif
