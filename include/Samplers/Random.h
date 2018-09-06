#ifndef __RANDOM_SAMPLER__
#define __RANDOM_SAMPLER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Sampler.h"

class Random: public Sampler {
	public:
		Random();
		Random(int num_samples);
		Random(int num_samples, int num_sets);
		Random(const Random& u);
		Random& operator=(const Random& rhs);
		Random* clone() const;
		virtual ~Random();
		void generate_samples();

};

#endif
