#ifndef __CONSTANT_COLOR__
#define __CONSTANT_COLOR__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Texture.h"

class ConstantColor: public Texture {
	public:
        // constructors, access functions, etc.
        ConstantColor();
        ConstantColor(const RGBColor& c);
        ConstantColor(const ConstantColor& cc);
        ConstantColor& operator= (const ConstantColor& rhs);
        virtual ~ConstantColor();
        virtual ConstantColor* clone() const;

        virtual RGBColor get_color(const ShadeRec& sr) const;

    private:

        RGBColor color;           // the color
};

inline RGBColor ConstantColor::get_color(const ShadeRec& sr) const {
    return color;
}

#endif
