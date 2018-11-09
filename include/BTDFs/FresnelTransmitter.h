#ifndef __FRESNEL_TRANSMITTER__
#define __FRESNEL_TRANSMITTER__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "BTDF.h"

class FresnelTransmitter: public BTDF {
    public:
        FresnelTransmitter();                         // default constructor
        virtual ~FresnelTransmitter();                // destructor
		FresnelTransmitter(const FresnelTransmitter& ft);         // copy constructor
        FresnelTransmitter& operator=(const FresnelTransmitter& rhs);// assignment operator
		virtual FresnelTransmitter* clone() const; // clone

        virtual RGBColor f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const;

        virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const;

        virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

        virtual bool tir(const ShadeRec& sr) const;

        virtual CLBRDF get_cl_btdf();

    private:
        float eta_in;
        float eta_out;
};

#endif
