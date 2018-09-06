// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Random
#include <cstdlib>

#include "Random.h"

Random::Random()
: Sampler() {
}

Random::Random(int num_samples)
: Sampler(num_samples) {
    generate_samples();
}

Random::Random(int num_samples, int num_sets)
: Sampler(num_samples, num_sets) {
    generate_samples();
}

Random::Random(const Random& u)
: Sampler(u) {
	generate_samples();
}

Random& Random::operator= (const Random& rhs) {
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Random* Random::clone() const {
	return (new Random(*this));
}

Random::~Random() {}

void Random::generate_samples(){
	for (int p = 0; p < num_sets; p++)
		for (int j = 0; j < num_samples; j++){
            	float rand1 = (float) rand() / RAND_MAX;
            	float rand2 = (float) rand() / RAND_MAX;
				Point2D sp(rand1, rand2);
				samples.push_back(sp);
			}
}
