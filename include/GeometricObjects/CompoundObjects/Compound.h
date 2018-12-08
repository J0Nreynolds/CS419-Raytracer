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

        virtual void add_object(GeometricObject* object_ptr);

        virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        virtual bool shadow_hit(const Ray& ray, float& tmin) const;

        virtual BBox get_bounding_box();

		virtual char get_cl_type() const;

    protected:
        BBox bbox;
        std::vector<GeometricObject*> objects;
};

inline std::vector<GeometricObject*>& Compound::get_objects(){
    return objects;
}

inline char Compound::get_cl_type() const {
    return 'c';
}

#endif
