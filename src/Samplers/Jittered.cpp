// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Jittered
#include <cstdlib>
#include <math.h>

#include "Jittered.h"

Jittered::Jittered()
: Sampler() {
}

Jittered::Jittered(int num_samples)
: Sampler(num_samples) {
    generate_samples();
}

Jittered::Jittered(int num_samples, int num_sets)
: Sampler(num_samples, num_sets) {
    generate_samples();
}

Jittered::Jittered(const Jittered& u)
: Sampler(u) {
	generate_samples();
}

Jittered& Jittered::operator= (const Jittered& rhs) {
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Jittered* Jittered::clone() const {
	return (new Jittered(*this));
}

Jittered::~Jittered() {}

void Jittered::generate_samples(){
	int n = (int) sqrt(num_samples);

	for (int p = 0; p < num_sets; p++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++) {
            	float rand1 = (float) rand() / RAND_MAX;
            	float rand2 = (float) rand() / RAND_MAX;
				Point2D sp((k + rand1) / n, (j + rand2) / n);
				samples.push_back(sp);
			}
}
