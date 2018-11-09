#ifndef __BTDF__
#define __BTDF__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Vector3D.h"
#include "ShadeRec.h"
#include "CLBRDF.h"

class BTDF {
    public:
        BTDF();                         // default constructor
        virtual ~BTDF();                // destructor
		BTDF(const BTDF& btdf);         // copy constructor
        BTDF& operator=(const BTDF& rhs);// assignment operator
		virtual BTDF* clone() const = 0; // clone

        virtual RGBColor f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const;

        virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const;

        virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

        virtual bool tir(const ShadeRec& sr) const = 0;

        virtual CLBRDF get_cl_btdf() = 0;
};

#endif
