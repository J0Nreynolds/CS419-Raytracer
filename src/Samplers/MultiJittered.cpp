// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class MultiJittered
#include <cstdlib>
#include <math.h>

#include "MultiJittered.h"

MultiJittered::MultiJittered()
: Sampler() {
}

MultiJittered::MultiJittered(int num_samples)
: Sampler(num_samples) {
    generate_samples();
}

MultiJittered::MultiJittered(int num_samples, int num_sets)
: Sampler(num_samples, num_sets) {
    generate_samples();
}

MultiJittered::MultiJittered(const MultiJittered& u)
: Sampler(u) {
	generate_samples();
}

MultiJittered& MultiJittered::operator= (const MultiJittered& rhs) {
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

MultiJittered* MultiJittered::clone() const {
	return (new MultiJittered(*this));
}

MultiJittered::~MultiJittered() {}

// This is based on code in Chui et al. (1994), cited in the references of book
// "Ray Tracing from the Ground Up"
void MultiJittered::generate_samples() {
	// num_samples needs to be a perfect square

	int n = (int)sqrt((float)num_samples);
	float subcell_width = 1.0 / ((float) num_samples);

	// fill the samples array with dummy points to allow us to use the [ ] notation when we set the
	// initial patterns

	Point2D fill_point;
	for (int j = 0; j < num_samples * num_sets; j++)
		samples.push_back(fill_point);

	// distribute points in the initial patterns

	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
                float rand1 = (float) rand() / RAND_MAX;
                float rand2 = (float) rand() / RAND_MAX;
				samples[i * n + j + p * num_samples].x = (i * n + j) * subcell_width + rand1 * subcell_width;
				samples[i * n + j + p * num_samples].y = (j * n + i) * subcell_width + rand2 * subcell_width;
			}

	// shuffle x coordinates

	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				int k = j + (int) rand() % (n - j);
				float t = samples[i * n + j + p * num_samples].x;
				samples[i * n + j + p * num_samples].x = samples[i * n + k + p * num_samples].x;
				samples[i * n + k + p * num_samples].x = t;
			}

	// shuffle y coordinates

	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				int k = j + (int) rand() % (n - j);
				float t = samples[j * n + i + p * num_samples].y;
				samples[j * n + i + p * num_samples].y = samples[k * n + i + p * num_samples].y;
				samples[k * n + i + p * num_samples].y = t;
		}
}
