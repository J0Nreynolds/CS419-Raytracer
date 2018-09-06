#ifndef __REGULAR_SAMPLER__
#define __REGULAR_SAMPLER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Sampler.h"

class Regular: public Sampler {
	public:
		Regular();
		Regular(int num_samples);
		Regular(const Regular& u);
		Regular& operator=(const Regular& rhs);
		Regular* clone() const;
		virtual ~Regular();
		void generate_samples();

};

#endif
