// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class IntersectCSG
#include <algorithm>
#include <iostream>
#include "IntersectCSG.h"

using namespace boost::icl;

IntersectCSG::IntersectCSG()
: CSG()
{}

IntersectCSG::IntersectCSG(CSG* l, CSG* r)
: CSG()
{
    left = l;
    right = r;
}

IntersectCSG::IntersectCSG(const IntersectCSG& icsg)
: CSG(icsg)
{
}

IntersectCSG& IntersectCSG::operator= (const IntersectCSG& rhs)
{
    CSG::operator=(rhs);
    return (*this);
}

IntersectCSG* IntersectCSG::clone() const
{
    return new IntersectCSG(*this);
}

IntersectCSG::~IntersectCSG()
{}

double IntersectCSG::f(double x, double y, double z){
    return std::max(left->f(x,y,z), right->f(x,y,z));
}

bool IntersectCSG::hit(const Ray& ray, double& t, ShadeRec& s) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);
    TIntervalSet times = left_times & right_times;
    double tmin = times.begin()->lower();
    if(times.begin() != times.end()){ // nonempty interval set
        for(auto it = left_times.begin(); it != left_times.end(); it ++){
            if(it->lower() == tmin && right->hit(ray, t, s)){
                material_ptr = left->get_material();
                return true;
            }
        }
        for(auto it = right_times.begin(); it != right_times.end(); it ++){
            if(it->lower() == tmin && left->hit(ray, t, s)){
                material_ptr = right->get_material();
                return true;
            }
        }
    }
    return false;
}

bool IntersectCSG::shadow_hit(const Ray& ray, float& tmin) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);
    TIntervalSet times = left_times & right_times;
    double t = times.begin()->lower();
    if(times.begin() != times.end()){ // nonempty interval set
        for(auto it = left_times.begin(); it != left_times.end(); it ++){
            if(it->lower() == t && right->shadow_hit(ray, tmin)){
                return true;
            }
        }
        for(auto it = right_times.begin(); it != right_times.end(); it ++){
            if(it->lower() == t && left->shadow_hit(ray, tmin)){
                return true;
            }
        }
    }
    return false;
}

TIntervalSet IntersectCSG::hit_times(const Ray& ray) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);

	return left_times & right_times;
}
