// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Hammersley
#include "Hammersley.h"

Hammersley::Hammersley()
: Sampler(1, 1) {
}

Hammersley::Hammersley(int num_samples)
: Sampler(num_samples, 1) {
	generate_samples();
}

Hammersley::Hammersley(int num_samples, int num_sets)
: Sampler(num_samples, 1) {
	generate_samples();
}

Hammersley::Hammersley(const Hammersley& u)
: Sampler(u) {
	generate_samples();
}

Hammersley& Hammersley::operator= (const Hammersley& rhs) {
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Hammersley* Hammersley::clone() const {
	return (new Hammersley(*this));
}

Hammersley::~Hammersley() {}

double phi (int j) {
	double x = 0.0;
	double f = 0.5;

	while (j) {
		x += f * (double) (j % 2);
		j /= 2;
		f *= 0.5;
	}

	return (x);
}

void Hammersley::generate_samples(){
	for (int p = 0; p < num_sets; p++)
		for (int j = 0; j < num_samples; j++){
			float x = (float) j / num_samples;
			float y = (float) phi(j);
			Point2D sp(x, y);
			samples.push_back(sp);
		}
}
