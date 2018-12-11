#ifndef __CUBIC_NOISE__
#define __CUBIC_NOISE__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

#include "LatticeNoise.h"

class CubicNoise : public LatticeNoise{
	 public:
		// constructors, access functions, etc.
		CubicNoise();
		CubicNoise(const CubicNoise& ln);
		CubicNoise& operator= (const CubicNoise& rhs);
		virtual ~CubicNoise();
		virtual CubicNoise* clone() const;

		// constructors, etc.
		virtual float value_noise(const Point3D& p) const;
};

#endif
