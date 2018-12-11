#ifndef __CHECKER_3D__
#define __CHECKER_3D__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Texture.h"

class Checker3D: public Texture {
	public:
        // constructors, access functions, etc.
        Checker3D();
        Checker3D(const RGBColor& c1, const RGBColor& c2);
        Checker3D(const RGBColor& c1, const RGBColor& c2, float s);
        Checker3D(const Checker3D& cc);
        Checker3D& operator= (const Checker3D& rhs);
        virtual ~Checker3D();
        virtual Checker3D* clone() const;

        virtual RGBColor get_color(const ShadeRec& sr) const;

    private:

        RGBColor color1;           // the first color
        RGBColor color2;           // the second color
		float size;				   // size of the checker pattern
};

#endif
