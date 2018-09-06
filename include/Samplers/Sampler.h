#ifndef __SAMPLER__
#define __SAMPLER__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include  <vector>

#include "Point2D.h"

class Sampler {
	 public:

			// constructors, access functions, etc.
			Sampler();
			Sampler(int num_samples);
			Sampler(int num_samples, int num_sets);
			Sampler(const Sampler& s);
			Sampler& operator= (const Sampler& rhs);
			virtual ~Sampler();
			int get_num_samples();
			void set_num_sets(const int n_sets);

			virtual void        // generate sample patterns in a unit square
			generate_samples() = 0;

			void                // set up the randomly shuffled indices
			setup_shuffled_indices();

			void                // randomly shuffle the samples in each pattern
			shuffle_samples();

			Point2D             // get next sample on unit square
			sample_unit_square();

	  protected:

			int num_samples;  // the number of sample points in a pattern
			int num_sets;     // the number of sample sets (patterns) stored
			std::vector<Point2D> samples; // sample points on a unit square
			std::vector<int> shuffled_indices;  // shuffled samples array indices
			unsigned long count;    // the current number of sample points used
			int jump;              // random index jump
};

inline int Sampler::get_num_samples(){
	return num_samples;
}

#endif
