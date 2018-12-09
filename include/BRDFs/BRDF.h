#ifndef __BRDF__
#define __BRDF__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "Vector3D.h"
#include "ShadeRec.h"
#include "Sampler.h"
#include "CLBRDF.h"

class BRDF {
    public:
        BRDF();                         // default constructor
        virtual ~BRDF();                // destructor
		BRDF(const BRDF& brdf);         // copy constructor
        BRDF& operator=(const BRDF& rhs);// assignment operator
		virtual BRDF* clone() const = 0; // clone

        void set_sampler_ptr(Sampler* ptr);
        Sampler* get_sampler_ptr();

        virtual RGBColor f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const;
        virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo, float& pdf) const;
        virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo) const;
        virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;

        virtual CLBRDF get_cl_brdf() = 0;

    protected:

        Sampler* sampler_ptr;
};

inline void BRDF::set_sampler_ptr(Sampler* ptr){
    if(sampler_ptr){
        delete sampler_ptr;
    }
    sampler_ptr = ptr;
}

inline Sampler* BRDF::get_sampler_ptr(){
    return sampler_ptr;
}

#endif
