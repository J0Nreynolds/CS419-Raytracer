// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class ViewPlane
#include "Regular.h"
#include "MultiJittered.h"

#include "ViewPlane.h"

void ViewPlane::set_sampler(Sampler* sp) {
		if (sampler_ptr) {
			delete sampler_ptr;
			sampler_ptr = NULL;
		}

	   num_samples = sp->get_num_samples();
	   sampler_ptr = sp;
}

void ViewPlane::set_samples(const int n) {
	   num_samples = n;

	   if (sampler_ptr) {
			delete sampler_ptr;
			sampler_ptr = NULL;
		}

	   if (num_samples > 1)
			sampler_ptr = new MultiJittered(num_samples);
	   else
			sampler_ptr = new Regular(1);
 }
