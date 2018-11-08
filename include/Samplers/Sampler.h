#ifndef __SAMPLER__
#define __SAMPLER__

// 	Copyright (C) Kevin Suffern 2000-2007.
//  Modified work Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include  <vector>

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

#include "Point2D.h"
#include "Point3D.h"
#include "CLSampler.h"

class Sampler {
	 public:

			// constructors, access functions, etc.
			Sampler();
			Sampler(int num_samples);
			Sampler(int num_samples, int num_sets);
			Sampler(const Sampler& s);
			Sampler& operator= (const Sampler& rhs);
			virtual ~Sampler();
			virtual Sampler* clone() const = 0;
			int get_num_samples();
			void set_num_sets(const int n_sets);

			// Used for OpenCL rendering
			int get_num_sets();
			cl_double2* get_cl_samples(int& count);
			cl_int* get_cl_shuffled_indices(int& count);
			cl_double2* get_cl_disk_samples(int& count);
			cl_double3* get_cl_hemisphere_samples(int& count);

			// generate sample patterns in a unit square
			virtual void generate_samples() = 0;
			// set up the randomly shuffled indices
			void setup_shuffled_indices();
			// randomly shuffle the samples in each pattern
			void shuffle_samples();
			// get next sample on unit square
			Point2D sample_unit_square();

			//Converts the unit square samples to unit disk samples
			void map_samples_to_unit_disk();
			// get next sample on unit disc
			Point2D sample_unit_disk();

			//Converts the unit square samples to hemisphere samples
			void map_samples_to_hemisphere(const float e);
			// get next sample on unit hemisphere
			Point3D sample_hemisphere();

	        void set_cl_index(int idx);
	        int get_cl_index();

	        void set_samples_index(int idx);
	        int get_samples_index();

	        void set_sampler_type(char type);
	        char get_sampler_type();

			CLSampler get_cl_sampler();

	  protected:

			int num_samples;  // the number of sample points in a pattern
			int num_sets;     // the number of sample sets (patterns) stored
			std::vector<Point2D> samples; // sample points on a unit square
			std::vector<int> shuffled_indices;  // shuffled samples array indices
			unsigned long count;    // the current number of sample points used
			int jump;              // random index jump

			std::vector<Point2D> disk_samples; // sample points on a unit disk
			std::vector<Point3D> hemisphere_samples;

	  private:

	        int cl_index;
	        int samples_index;
	        char sampler_type;
};

inline int Sampler::get_num_samples(){
	return num_samples;
}

inline int Sampler::get_num_sets(){
	return num_sets;
}

inline void Sampler::set_cl_index(int idx){
    cl_index = idx;
}

inline int Sampler::get_cl_index(){
    return cl_index;
}

inline void Sampler::set_samples_index(int idx){
    samples_index = idx;
}

inline int Sampler::get_samples_index(){
    return samples_index;
}


inline void Sampler::set_sampler_type(char type){
	sampler_type = type;
}

inline char Sampler::get_sampler_type(){
	return sampler_type;
}

#endif
