// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Sampler
#include <cstdlib>
#include <time.h>

#include "Sampler.h"

Sampler::Sampler ()
	:	num_samples(1),
		num_sets(83),
		count(0),
		jump(0)
{
	samples.reserve(num_samples * num_sets);
}

Sampler::Sampler (int ns)
	:	num_samples(ns),
		num_sets(83),
		count(0),
		jump(0)
{
	samples.reserve(num_samples * num_sets);
}

Sampler::Sampler (int ns, int n_sets)
	:	num_samples(ns),
		num_sets(n_sets),
		count(0),
		jump(0)
{
	samples.reserve(num_samples * num_sets);
}

Sampler::Sampler(const Sampler& s)
	: 	num_samples(s.num_samples),
		num_sets(s.num_sets),
		samples(s.samples),
		shuffled_indices(s.shuffled_indices),
		count(s.count),
		jump(s.jump)
{}

Sampler& Sampler::operator= (const Sampler& rhs)	{
	if (this == &rhs)
		return (*this);

	num_samples 		= rhs.num_samples;
	num_sets			= rhs.num_sets;
	samples				= rhs.samples;
	shuffled_indices	= rhs.shuffled_indices;
	count				= rhs.count;
	jump				= rhs.jump;

	return (*this);
}

Sampler::~Sampler() {}

void Sampler::set_num_sets(const int n_sets) {
	num_sets = n_sets;
}

Point2D Sampler::sample_unit_square() {
	//old
	// return (samples[count++ % (num_samples * num_sets)]);

	//new
	 if (count % num_samples == 0)      // start of a new pixel
		  jump = ((int) rand() % num_sets) * num_samples;

	 return (samples[jump + count++ % num_samples]);
}
