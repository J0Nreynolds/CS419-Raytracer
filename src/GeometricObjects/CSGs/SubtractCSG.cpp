// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class SubtractCSG
#include <algorithm>
#include "SubtractCSG.h"

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
    return false;
}

bool SubtractCSG::shadow_hit(const Ray& ray, float& tmin) const{
    return false;
}

std::vector<float> subtract_times(std::vector<float> l, std::vector<float> r){
}

std::vector<float> SubtractCSG::hit_times(const Ray& ray) const{
    std::vector<float> left_times = left->hit_times(ray);
    std::vector<float> right_times = right->hit_times(ray);

	return subtract_times(left_times, right_times);
}
