// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class UnionCSG
#include <algorithm>
#include <boost/numeric/interval.hpp>

#include "UnionCSG.h"

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
    return false;
}

bool UnionCSG::shadow_hit(const Ray& ray, float& tmin) const{
    return false;
}

std::vector<float> union_times(std::vector<float> l, std::vector<float> r){
    return std::vector<float>();
}

std::vector<float> UnionCSG::hit_times(const Ray& ray) const{
    std::vector<float> left_times = left->hit_times(ray);
    std::vector<float> right_times = right->hit_times(ray);

	return union_times(left_times, right_times);
}
