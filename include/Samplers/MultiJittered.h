#ifndef __MULTI_JITTERED_SAMPLER__
#define __MULTI_JITTERED_SAMPLER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Sampler.h"

class MultiJittered: public Sampler {
	public:
		MultiJittered();
		MultiJittered(int num_samples);
		MultiJittered(int num_samples, int num_sets);
		MultiJittered(const MultiJittered& u);
		MultiJittered& operator=(const MultiJittered& rhs);
		virtual MultiJittered* clone() const;
		virtual ~MultiJittered();
		void generate_samples();

};

#endif
