// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class NRooks
#include <cstdlib>

#include "NRooks.h"

NRooks::NRooks()
: Sampler() {
}

NRooks::NRooks(int num_samples)
: Sampler(num_samples) {
	generate_samples();
}

NRooks::NRooks(int num_samples, int num_sets)
: Sampler(num_samples, num_sets) {
	generate_samples();
}

NRooks::NRooks(const NRooks& u)
: Sampler(u) {
	generate_samples();
}

NRooks& NRooks::operator= (const NRooks& rhs) {
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

NRooks* NRooks::clone() const {
	return (new NRooks(*this));
}

NRooks::~NRooks() {}

void NRooks::generate_samples(){
	for (int p = 0; p < num_sets; p++)
		for (int j = 0; j < num_samples; j++){
				float rand1 = (float) rand() / RAND_MAX;
				float rand2 = (float) rand() / RAND_MAX;
				Point2D pv;
				pv.x = (j + rand1) / num_samples;
				pv.y = (j + rand2) / num_samples;
				samples.push_back(pv);
		}

	shuffle_x_coordinates();
	shuffle_y_coordinates();
}

void NRooks::shuffle_x_coordinates() {
    for (int p = 0; p < num_sets; p++)
        for (int i = 0; i <  num_samples - 1; i++) {
			int rand_int = (int) rand();
            int target = rand_int % num_samples + p * num_samples;
            float temp = samples[i + p * num_samples + 1].x;
            samples[i + p * num_samples + 1].x = samples[target].x;
            samples[target].x = temp;
		}
}

void NRooks::shuffle_y_coordinates() {
    for (int p = 0; p < num_sets; p++)
        for (int i = 0; i <  num_samples - 1; i++) {
			int rand_int = (int) rand();
            int target = rand_int % num_samples + p * num_samples;
            float temp = samples[i + p * num_samples + 1].y;
            samples[i + p * num_samples + 1].y = samples[target].y;
            samples[target].y = temp;
		}
}
