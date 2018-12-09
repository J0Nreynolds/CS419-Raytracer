#ifndef __SV_LAMBERTIAN__
#define __SV_LAMBERTIAN__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include "BRDF.h"
#include "Texture.h"

class SV_Lambertian: public BRDF {
	public:
		SV_Lambertian();
		SV_Lambertian(float kd);
		SV_Lambertian(Texture* cd);
		SV_Lambertian(float kd, Texture* cd);
		virtual ~SV_Lambertian();
		SV_Lambertian(const SV_Lambertian& svl);
		SV_Lambertian& operator=(const SV_Lambertian& rhs);
		virtual SV_Lambertian* clone() const;

		void set_kd(float kd_new);
		float get_kd();

		void set_cd(Texture* tex);
		Texture* get_cd();

		virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo) const;
		virtual RGBColor f(const ShadeRec& sr, const Vector3D& wi, const Vector3D& wo) const;
		virtual RGBColor sample_f(const ShadeRec& sr, Vector3D& wi, const Vector3D& wo, float& pdf) const;

        virtual CLBRDF get_cl_brdf();

	private:

		float kd;
		Texture* cd;
};

inline void SV_Lambertian::set_kd(float kd_new){
	kd = kd_new;
}

inline float SV_Lambertian::get_kd(){
	return kd;
}

inline void SV_Lambertian::set_cd(Texture* tex){
	delete cd;
	cd = tex;
}

inline Texture* SV_Lambertian::get_cd(){
	return cd;
}

#endif
