// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class CSG
#include "IntersectCSG.h"
#include "UnionCSG.h"
#include "SubtractCSG.h"
#include "Constants.h"
#include "CSG.h"


CSG::CSG()
: left(NULL), right(NULL)
{}

CSG::CSG(const CSG& csg)
{
    if(csg.left != NULL)
        left = csg.left->clone();
    if(csg.right != NULL)
        right = csg.right->clone();
}

CSG& CSG::operator=(const CSG& rhs)
{
    if(left){
        delete left;
        left = NULL;
    }
    if(rhs.left != NULL)
        left = rhs.left->clone();

    if(right){
        delete right;
        right = NULL;
    }
    if(rhs.right != NULL)
        right = rhs.right->clone();
    return (*this);
}

CSG::~CSG()
{
    if(left != NULL)
        delete left;
    if(right != NULL)
        delete right;
}

CSG* CSG::intersect(const CSG* other) const{
    return new IntersectCSG((CSG*)this, (CSG*)other);
}

CSG* CSG::union_with(const CSG* other) const{
    return new UnionCSG((CSG*)this, (CSG*)other);
}

CSG* CSG::subtract(const CSG* other) const{
    return new SubtractCSG((CSG*)this, (CSG*)other);
}
