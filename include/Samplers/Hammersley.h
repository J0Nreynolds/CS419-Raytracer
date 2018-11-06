#ifndef __HAMMERSLEY_SAMPLER__
#define __HAMMERSLEY_SAMPLER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Sampler.h"

class Hammersley: public Sampler {
	public:
		Hammersley();
		Hammersley(int num_samples);
		Hammersley(int num_samples, int num_sets);
		Hammersley(const Hammersley& u);
		Hammersley& operator=(const Hammersley& rhs);
		virtual Hammersley* clone() const;
		virtual ~Hammersley();
		void generate_samples();

};

#endif
