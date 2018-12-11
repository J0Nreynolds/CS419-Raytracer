#ifndef __LINEAR_NOISE__
#define __LINEAR_NOISE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

#include "LatticeNoise.h"

class LinearNoise : public LatticeNoise{
	 public:
		// constructors, access functions, etc.
		LinearNoise();
		LinearNoise(const LinearNoise& ln);
		LinearNoise& operator= (const LinearNoise& rhs);
		virtual ~LinearNoise();
		virtual LinearNoise* clone() const;

		// constructors, etc.
		virtual float value_noise(const Point3D& p) const;
};

#endif
