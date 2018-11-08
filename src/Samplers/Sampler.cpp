// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Sampler
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <algorithm>  // required for random_shuffle

#include <iostream>

#include "Constants.h"
#include "Sampler.h"

Sampler::Sampler ()
	:	num_samples(1),
		num_sets(83),
		count(0),
		jump(0)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}

Sampler::Sampler (int ns)
	:	num_samples(ns),
		num_sets(83),
		count(0),
		jump(0)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}

Sampler::Sampler (int ns, int n_sets)
	:	num_samples(ns),
		num_sets(n_sets),
		count(0),
		jump(0)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
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

void Sampler::setup_shuffled_indices(void) {
	shuffled_indices.reserve(num_samples * num_sets);
	std::vector<int> indices;

	for (int j = 0; j < num_samples; j++)
		indices.push_back(j);

	for (int p = 0; p < num_sets; p++) {
		std::random_shuffle(indices.begin(), indices.end());
		for (int j = 0; j < num_samples; j++)
			shuffled_indices.push_back(indices[j]);
	}
}

Point2D Sampler::sample_unit_square() {
	if (count % num_samples == 0)
		jump = ((int) rand() % num_sets) * num_samples;

	return (samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}

void Sampler::map_samples_to_unit_disk() {
	int size = samples.size();
	float r, phi;                                    // polar coordinates
	Point2D sp;                                      // sample point on unit disk

	disk_samples.resize(size);

	for (int j = 0; j < size; j++) {
		// map sample point to [-1, 1]  [-1,1]

		sp.x = 2.0 * samples[j].x - 1.0;
		sp.y = 2.0 * samples[j].y - 1.0;

		if (sp.x > -sp.y) {                        // sectors 1 and 2
				if (sp.x > sp.y) {                   // sector 1
					r = sp.x;
					phi = sp.y / sp.x;
				}
				else {                               // sector 2
					r = sp.y;
					phi = 2 - sp.x / sp.y;
				}
		}
		else {                                     // sectors 3 and 4
				if (sp.x < sp.y) {                   // sector 3
					r = -sp.x;
					phi = 4 + sp.y / sp.x;
				}
				else {                               // sector 4
					r = -sp.y;
					if (sp.y != 0.0)               // avoid division by zero at origin
						phi = 6 - sp.x / sp.y;
					else
						phi = 0.0;
				}
		}

		  phi *= PI / 4.0;

		  disk_samples[j].x = r * cos(phi);
		  disk_samples[j].y = r * sin(phi);
	}
}

Point2D Sampler::sample_unit_disk() {
	 if (count % num_samples == 0)      // start of a new pixel
		  jump = ((int) rand() % num_sets) * num_samples;

	 return (disk_samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}

void Sampler::map_samples_to_hemisphere(const float e) {
    int size = samples.size();
    hemisphere_samples.reserve(num_samples * num_sets);
    for (int j = 0; j < size; j++) {
        float cos_phi = cos(2.0 * PI * samples[j].x);
        float sin_phi = sin(2.0 * PI * samples[j].x);
        float cos_theta = pow((1.0 - samples[j].y), 1.0 / (e + 1.0));
        float sin_theta = sqrt (1.0 - cos_theta * cos_theta);
        float pu = sin_theta * cos_phi;
        float pv = sin_theta * sin_phi;
        float pw = cos_theta;
        hemisphere_samples.push_back(Point3D(pu, pv, pw));
    }
}

Point3D Sampler::sample_hemisphere() {
	if (count % num_samples == 0)      // start of a new pixel
		jump = ((int) rand() % num_sets) * num_samples;

	return (hemisphere_samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}

cl_double2* Sampler::get_cl_samples(int& count) {
	count = samples.size();
	cl_double2* cl_samples = new cl_double2[count];
	for(int i = 0; i < count; i ++){
		cl_samples[i] = (cl_double2){samples[i].x, samples[i].y};
	}
	return cl_samples;
}

cl_double2* Sampler::get_cl_disk_samples(int& count) {
	count = disk_samples.size();
	cl_double2* cl_disk_samples = new cl_double2[count];
	for(int i = 0; i < count; i ++){
		cl_disk_samples[i] = (cl_double2){disk_samples[i].x, disk_samples[i].y};
	}
	return cl_disk_samples;
}

cl_double3* Sampler::get_cl_hemisphere_samples(int& count) {
	count = hemisphere_samples.size();
	cl_double3* cl_hemisphere_samples = new cl_double3[count];
	for(int i = 0; i < count; i ++){
		cl_hemisphere_samples[i] = (cl_double3){hemisphere_samples[i].x, hemisphere_samples[i].y, hemisphere_samples[i].z};
	}
	return cl_hemisphere_samples;
}

cl_int* Sampler::get_cl_shuffled_indices(int& count) {
	count = shuffled_indices.size();
	cl_int* cl_indices = new cl_int[count];
	for(int i = 0; i < count; i ++){
		cl_indices[i] = shuffled_indices[i];
	}
	return cl_indices;
}

CLSampler Sampler::get_cl_sampler(){
	CLSampler ret;
	ret.num_samples = (cl_int)num_samples;
	ret.num_sets = (cl_int)num_sets;
	ret.samples_index = (cl_int)samples_index;
	ret.sampler_type = (cl_char)sampler_type;
	return ret;
}
