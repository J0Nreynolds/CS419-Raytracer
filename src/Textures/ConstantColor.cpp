// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class ConstantColor
#include "Constants.h"
#include "ConstantColor.h"


ConstantColor::ConstantColor()
: color(black)
{}

ConstantColor::ConstantColor(const RGBColor& c)
: color(c)
{}

ConstantColor::ConstantColor(const ConstantColor& cc)
: color(cc.color)
{}

ConstantColor& ConstantColor::operator= (const ConstantColor& rhs)
{
    color = rhs.color;
}

ConstantColor::~ConstantColor()
{}

ConstantColor* ConstantColor::clone() const
{
    return new ConstantColor(*this);
}
