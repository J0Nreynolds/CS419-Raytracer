// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.

// This file contains the definition of the class Compound

#include "Compound.h"

Compound::Compound()
: GeometricObject()
{}

Compound::Compound(const Compound& compound)
: GeometricObject(compound) {
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
    GeometricObject::operator=(compound);
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
}

bool Compound::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    double t;
    Normal normal;
    Point3D local_hit_point;
    bool hit = false;
    tmin = kHugeValue;
    int num_objects = objects.size();

    for (int j = 0; j < num_objects; j++){
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            hit = true;
            tmin = t;
            normal = sr.normal;
            local_hit_point = sr.local_hit_point;
            material_ptr = (Material*) objects[j]->get_material();
        }
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


void Compound::add_object(GeometricObject* object_ptr){
    BBox obj_bbox = ((GeometricObject*)object_ptr)->get_bounding_box();
    if(objects.size() == 0){ //if we have no bounding box
        bbox = obj_bbox;
    }
    else {
        bbox.x0 = std::min(bbox.x0, obj_bbox.x0);
        bbox.y0 = std::min(bbox.y0, obj_bbox.y0);
        bbox.z0 = std::min(bbox.z0, obj_bbox.z0);
        bbox.x1 = std::max(bbox.x1, obj_bbox.x1);
        bbox.y1 = std::max(bbox.y1, obj_bbox.y1);
        bbox.z1 = std::max(bbox.z1, obj_bbox.z1);
    }
    objects.push_back((GeometricObject*) object_ptr);
}


BBox Compound::get_bounding_box(){
    return bbox;
}
