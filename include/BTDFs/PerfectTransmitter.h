#ifndef __PERFECT_TRANSMITTER__
#define __PERFECT_TRANSMITTER__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "BTDF.h"

class PerfectTransmitter: public BTDF {
    public:
        PerfectTransmitter();                         // default constructor
        virtual ~PerfectTransmitter();                // destructor
		PerfectTransmitter(const PerfectTransmitter& pt);         // copy constructor
        PerfectTransmitter& operator=(const PerfectTransmitter& rhs);// assignment operator
		virtual PerfectTransmitter* clone() const; // clone

        void set_kt(float kr);
        void set_ior(float ior);

        virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const;

        virtual bool tir(const ShadeRec& sr) const;

        virtual CLBRDF get_cl_btdf();

    private:
        float kt;
        float ior;
};

inline void PerfectTransmitter::set_kt(float k){
    kt = k;
}

inline void PerfectTransmitter::set_ior(float i){
    ior = i;
}

#endif
