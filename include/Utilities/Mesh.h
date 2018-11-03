#ifndef __MESH__
#define __MESH__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.
#include <vector>
#include "Point3D.h"
#include "Normal.h"

#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#include <cl.hpp> /* read cpp_wrapper_fix.txt */
#else
#include <CL/cl.hpp>
#endif

class Mesh {
    public:
        std::vector<Point3D> vertices;           // vertices
        std::vector<Normal> normals;             // average normal at each vertex
        std::vector< std::vector<int> > vertex_faces;   // the faces shared by each vertex
        std::vector<float> u;            // u texture coordinate at each vertex
        std::vector<float> v;            // v texture coordinate at each vertex
        int num_vertices;           // number of vertices
        int num_triangles;          // number of triangles

        // constructors, etc.
        Mesh(void);
        Mesh(const Mesh& m);
        ~Mesh(void);
        Mesh& operator= (const Mesh& rhs);

        void set_cl_index(int idx);
        int get_cl_index();

        cl_double3* get_cl_vertices();
        cl_double3* get_cl_normals();

    private:
        int cl_index;
};

inline void Mesh::set_cl_index(int idx){
    cl_index = idx;
}

inline int Mesh::get_cl_index(){
    return cl_index;
}

#endif
