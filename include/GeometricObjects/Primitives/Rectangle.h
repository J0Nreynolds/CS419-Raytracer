#ifndef __RECTANGLE__
#define __RECTANGLE__

// 	Copyright (C) Kevin Suffern 2000-2007.
// 	Modified work (C) Jonathan Reynolds 2018.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the declaration of the class Rectangle

#include "Sampler.h"
#include "GeometricObject.h"

//-------------------------------------------------------------------------------- class Rectangle

class Rectangle: public GeometricObject {

     public:

        // constructors, access functions, hit functions
        Rectangle();

		Rectangle(const Point3D& p0, const Vector3D& a, const Vector3D& b);

		Rectangle(const Point3D& p0, const Vector3D& a, const Vector3D& b, const Normal& n);

		virtual Rectangle* clone() const;

		Rectangle(const Rectangle& r);

		virtual ~Rectangle();

		Rectangle& operator= (const Rectangle& rhs);

		BBox get_bounding_box(void);

		virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;

		virtual bool shadow_hit(const Ray& ray, float& tmin) const;

        void set_sampler(Sampler* sampler);

        virtual Point3D sample(void);

        virtual float pdf(const ShadeRec& sr);

        virtual Normal get_normal(const Point3D& p);

    private:

		Point3D 		p0;   			// corner vertex
		Vector3D		a;				// side
		Vector3D		b;				// side
		double			a_len_squared;	// square of the length of side a
		double			b_len_squared;	// square of the length of side b
		Normal			normal;

		float			area;			// for rectangular lights
		float			inv_area;		// for rectangular lights
		Sampler*		sampler_ptr;	// for rectangular lights

        static const double kEpsilon;   // for shadows and secondary rays

};

#endif
