#ifndef __TEXTURE__
#define __TEXTURE__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "ShadeRec.h"

class Texture {
	 public:
			// constructors, access functions, etc.
			Texture();
			Texture(const Texture& t);
			Texture& operator= (const Texture& rhs);
			virtual ~Texture();
			virtual Texture* clone() const = 0;

			virtual RGBColor get_color(const ShadeRec& sr) const = 0;
};

#endif
