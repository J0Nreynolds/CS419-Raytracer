#ifndef __COMPOUND__
#define __COMPOUND__
// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include <vector>
#include "GeometricObject.h"

class Compound: public GeometricObject {
    public:

        Compound();
        Compound(const Compound& object);
		virtual Compound* clone() const;
		virtual ~Compound();

		std::vector<GeometricObject*>& get_objects();

		Compound& operator=(const Compound& compound);

        virtual void set_material(const Material* material_ptr);

        void add_object(const GeometricObject* object_ptr);

        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        virtual bool shadow_hit(const Ray& ray, float& tmin) const;

    protected:

        std::vector<GeometricObject*> objects;
};

inline void Compound::add_object(const GeometricObject* object_ptr){
    objects.push_back((GeometricObject*) object_ptr);
}

inline std::vector<GeometricObject*>& Compound::get_objects(){
    return objects;
}

#endif
