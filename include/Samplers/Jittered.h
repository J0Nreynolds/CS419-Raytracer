#ifndef __JITTERED_SAMPLER__
#define __JITTERED_SAMPLER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Sampler.h"

class Jittered: public Sampler {
	public:
		Jittered();
		Jittered(int num_samples);
		Jittered(int num_samples, int num_sets);
		Jittered(const Jittered& u);
		Jittered& operator=(const Jittered& rhs);
		Jittered* clone() const;
		virtual ~Jittered();
		void generate_samples();

};

#endif
