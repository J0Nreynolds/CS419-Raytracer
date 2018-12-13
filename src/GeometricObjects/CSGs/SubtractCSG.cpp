// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class SubtractCSG
#include <algorithm>
#include <iostream>
#include "SubtractCSG.h"

using namespace boost::icl;

SubtractCSG::SubtractCSG()
: CSG()
{}

SubtractCSG::SubtractCSG(CSG* l, CSG* r)
: CSG()
{
    left = l;
    right = r;
}

SubtractCSG::SubtractCSG(const SubtractCSG& scsg)
: CSG(scsg)
{
}

SubtractCSG& SubtractCSG::operator= (const SubtractCSG& rhs)
{
    CSG::operator=(rhs);
    return (*this);
}

SubtractCSG* SubtractCSG::clone() const
{
    return new SubtractCSG(*this);
}

SubtractCSG::~SubtractCSG()
{}

double SubtractCSG::f(double x, double y, double z){
    return std::max(left->f(x,y,z), -right->f(x,y,z));
}

bool SubtractCSG::hit(const Ray& ray, double& t, ShadeRec& s) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);
    TIntervalSet times = left_times - right_times;
    double tmin = times.begin()->lower();
    if(times.begin() != times.end()){ // nonempty interval set
        for(auto it = left_times.begin(); it != left_times.end(); it ++){
            if(it->lower() == tmin && left->hit(ray, t, s)){
                material_ptr = left->get_material();
                return true;
            }
        }
        for(auto it = right_times.begin(); it != right_times.end(); it ++){
            if(it->upper() == tmin && right->hit(ray, t, s)){
                s.normal = - s.normal;
                material_ptr = right->get_material();
                return true;
            }
        }
    }
    return false;
}

bool SubtractCSG::shadow_hit(const Ray& ray, float& tmin) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);
    TIntervalSet times = left_times - right_times;
    double t = times.begin()->lower();
    if(times.begin() != times.end()){ // nonempty interval set
        for(auto it = left_times.begin(); it != left_times.end(); it ++){
            if(it->lower() == t && left->shadow_hit(ray, tmin)){
                return true;
            }
        }
        for(auto it = right_times.begin(); it != right_times.end(); it ++){
            if(it->upper() == t && right->shadow_hit(ray, tmin)){
                return true;
            }
        }
    }
    return false;
}

TIntervalSet SubtractCSG::hit_times(const Ray& ray) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);

	return left_times - right_times;
}
