#ifndef __MULTIPLE_OBJECTS__
#define __MULTIPLE_OBJECTS__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


#include "Tracer.h"

class MultipleObjects: public Tracer {
	public:

		MultipleObjects(void);

		MultipleObjects(World* _worldPtr);

		virtual
		~MultipleObjects(void);

		virtual RGBColor
		trace_ray(const Ray& ray) const;
};

#endif
