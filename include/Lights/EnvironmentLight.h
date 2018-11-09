#ifndef __ENVIRONMENT_LIGHT__
#define __ENVIRONMENT_LIGHT__

// 	Copyright (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the declaration of the class EnvironmentLight
class EnvironmentLight: public Light {
     public:
		EnvironmentLight();				      // default constructor:
   		EnvironmentLight(const EnvironmentLight& al);   // copy constructor
		EnvironmentLight& operator= (const EnvironmentLight& rhs); // assignment operator
		virtual ~EnvironmentLight();             // deconstructor
        virtual EnvironmentLight* clone() const; // clone

        void set_material(Material* mat);
        void set_sampler(Sampler* sampler);

        virtual Vector3D get_direction(ShadeRec& s);

        virtual RGBColor L(ShadeRec& sr);

        bool in_shadow(const Ray& ray, const ShadeRec& sr) const;

        virtual float pdf(const ShadeRec& sr) const;

        virtual CLLight get_cl_light();

     private:

          Sampler* sampler_ptr;
          Material* material_ptr;
          Vector3D u, v, w;
          Vector3D wi;
};

inline void EnvironmentLight::set_sampler(Sampler* s_ptr){
    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    sampler_ptr = s_ptr;
    sampler_ptr->map_samples_to_hemisphere(1);
}

inline void EnvironmentLight::set_material(Material* mat){
    if(material_ptr){
        delete material_ptr;
        material_ptr = NULL;
    }
    if(mat){
        material_ptr = mat;
    }
}

#endif
