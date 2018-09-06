#ifndef __N_ROOKS_SAMPLER__
#define __N_ROOKS_SAMPLER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Sampler.h"

class NRooks: public Sampler {
	public:
		NRooks();
		NRooks(int num_samples);
		NRooks(int num_samples, int num_sets);
		NRooks(const NRooks& u);
		NRooks& operator=(const NRooks& rhs);
		NRooks* clone() const;
		virtual ~NRooks();
		void generate_samples();
		void shuffle_x_coordinates();
		void shuffle_y_coordinates();

};

#endif
