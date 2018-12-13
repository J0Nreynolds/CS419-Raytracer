// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class UnionCSG
#include <algorithm>
#include <iostream>
#include "UnionCSG.h"

using namespace boost::icl;

UnionCSG::UnionCSG()
: CSG()
{}

UnionCSG::UnionCSG(CSG* l, CSG* r)
: CSG()
{
    left = l;
    right = r;
}

UnionCSG::UnionCSG(const UnionCSG& ucsg)
: CSG(ucsg)
{
}

UnionCSG& UnionCSG::operator= (const UnionCSG& rhs)
{
    CSG::operator=(rhs);
    return (*this);
}

UnionCSG* UnionCSG::clone() const
{
    return new UnionCSG(*this);
}

UnionCSG::~UnionCSG()
{}

double UnionCSG::f(double x, double y, double z){
    return std::min(left->f(x,y,z), right->f(x,y,z));
}

bool UnionCSG::hit(const Ray& ray, double& t, ShadeRec& s) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);
    TIntervalSet times = left_times + right_times;
    double tmin = times.begin()->lower();
    if(times.begin() != times.end()){ // nonempty interval set
        for(auto it = left_times.begin(); it != left_times.end(); it ++){
            if(it->lower() == tmin && left->hit(ray, t, s)){
                material_ptr = left->get_material();
                return true;
            }
        }
        for(auto it = right_times.begin(); it != right_times.end(); it ++){
            if(it->lower() == tmin && right->hit(ray, t, s)){
                material_ptr = right->get_material();
                return true;
            }
        }
    }
    return false;
}

bool UnionCSG::shadow_hit(const Ray& ray, float& tmin) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);
    TIntervalSet times = left_times + right_times;
    double t = times.begin()->lower();
    if(times.begin() != times.end()){ // nonempty interval set
        for(auto it = left_times.begin(); it != left_times.end(); it ++){
            if(it->lower() == t && left->shadow_hit(ray, tmin)){
                return true;
            }
        }
        for(auto it = right_times.begin(); it != right_times.end(); it ++){
            if(it->lower() == t && right->shadow_hit(ray, tmin)){
                return true;
            }
        }
    }
    return false;
}

TIntervalSet UnionCSG::hit_times(const Ray& ray) const{
    TIntervalSet left_times = left->hit_times(ray);
    TIntervalSet right_times = right->hit_times(ray);

	return left_times + right_times;
}
