#ifndef __LATTICE_NOISE__
#define __LATTICE_NOISE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

#include "Point3D.h"

const int kTableSize = 256;
const int kTableMask = kTableSize - 1;

class LatticeNoise {
	 public:
		// constructors, access functions, etc.
		LatticeNoise();
		LatticeNoise(const LatticeNoise& ln);
		LatticeNoise& operator= (const LatticeNoise& rhs);
		virtual ~LatticeNoise();
		virtual LatticeNoise* clone() const = 0;

		void set_fs_min(float fmin);
		void set_fs_max(float fmax);
		void set_gain(float g);
		void set_lacunarity(float l);
		void set_fBm_min(float fmin);
		void set_fBm_max(float fmax);
		void set_num_octaves(int num);

		// constructors, etc.
		virtual float value_noise(const Point3D& p) const = 0;
		float value_fractal_sum(const Point3D& p) const;
		float value_turbulence(const Point3D& p) const;
		float value_fBm(const Point3D& p) const;

	 protected:
		static const unsigned char permutation_table[kTableSize];
		float value_table[kTableSize];

	 private:
		void init_value_table();
  		float fs_min;
  		float fs_max;
  		float gain;
  		float lacunarity;
  		float fBm_min;
  		float fBm_max;
  		int num_octaves;
};

#define PERM(x) permutation_table[(x)&kTableMask]
#define INDEX(ix,iy,iz) PERM((ix)+PERM((iy)+PERM(iz)))

inline void LatticeNoise::set_fs_min(float fmin){
	fs_min = fmin;
}

inline void LatticeNoise::set_fs_max(float fmax){
	fs_max = fmax;
}

inline void LatticeNoise::set_gain(float g){
	gain = g;
}

inline void LatticeNoise::set_lacunarity(float l){
	lacunarity = l;
}

inline void LatticeNoise::set_fBm_min(float fmin){
	fBm_min = fmin;
}

inline void LatticeNoise::set_fBm_max(float fmax){
	fBm_max = fmax;
}

inline void LatticeNoise::set_num_octaves(int num){
	num_octaves = num;
}

#endif
