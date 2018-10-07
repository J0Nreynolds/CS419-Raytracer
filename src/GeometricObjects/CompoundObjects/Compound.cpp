// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class Compound

#include "Compound.h"

Compound::Compound(){}

Compound::Compound(const Compound& compound){
    for(GeometricObject* object : compound.objects){
		if(object != NULL)
    		add_object(object->clone());
    }
}

Compound* Compound::clone() const {
    Compound* ret = new Compound(*this);
    return ret;
}

Compound::~Compound(){
    for(GeometricObject* object : objects){
		if(object != NULL)
			delete object;
    }
    objects.clear();
}

Compound& Compound::operator=(const Compound& compound){
    for(GeometricObject* object : objects){
		if(object != NULL)
			delete object;
    }
    objects.clear();
    for(GeometricObject* object : compound.objects){
		if(object != NULL)
    		add_object(object->clone());
    }
    return (*this);
}

void Compound::set_material(const Material* material_ptr) {
    int num_objects = objects.size();
    for (int j = 0; j < num_objects; j++)
        objects[j]->set_material((Material*) material_ptr);
    this->material_ptr = (Material*) material_ptr;
}


bool Compound::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    double t;
    Normal normal;
    Point3D local_hit_point;
    bool hit = false;
    tmin = kHugeValue;
    int num_objects = objects.size();

    for (int j = 0; j < num_objects; j++)
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            hit = true;
            tmin = t;
            normal = sr.normal;
            local_hit_point = sr.local_hit_point;
        }

    if (hit) {
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }

    return (hit);
}

bool Compound::shadow_hit(const Ray& ray, float& tmin) const {
    float t;
    bool hit = false;
    tmin = kHugeValue;
    int num_objects = objects.size();

    for (int j = 0; j < num_objects; j++)
        if (objects[j]->shadow_hit(ray, t) && (t < tmin)) {
            hit = true;
            tmin = t;
        }

    return (hit);
}
