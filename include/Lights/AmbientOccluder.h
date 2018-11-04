#ifndef __AMBIENT_OCCLUDER__
#define __AMBIENT_OCCLUDER__

#include "Vector3D.h"
#include "Ambient.h"
#include "Sampler.h"
#include "World.h"

class AmbientOccluder: public Ambient {
	public:
		AmbientOccluder();				  // default constructor
		AmbientOccluder(RGBColor color);  // color constructor
		AmbientOccluder(RGBColor color, float min_amount);  // color and min_amount constructor
		virtual ~AmbientOccluder();       // default deconstructor

        void set_sampler(Sampler* s_ptr);
		Sampler* get_sampler();
		
		void set_min_amount(float amount);

        virtual CLLight get_cl_light();
		virtual Vector3D get_direction(ShadeRec sr);
		virtual RGBColor L(ShadeRec sr);
		virtual bool in_shadow(const Ray& ray, const ShadeRec& sr) const;

	private:
        Vector3D u, v, w;
        Sampler* sampler_ptr;
        RGBColor min_amount;
};

inline void AmbientOccluder::set_sampler(Sampler* s_ptr){
    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    sampler_ptr = s_ptr;
    sampler_ptr->map_samples_to_hemisphere(1);
}

inline Sampler* AmbientOccluder::get_sampler(){
	return sampler_ptr;
}

inline void AmbientOccluder::set_min_amount(float amount){
	min_amount = RGBColor(amount);
}

#endif
