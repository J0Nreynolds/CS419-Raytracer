#ifndef __AREA_LIGHTING__
#define __AREA_LIGHTING__

#include "Tracer.h"

class AreaLighting : public Tracer {
	public:

		AreaLighting();

		AreaLighting(World* _world_ptr);

		virtual
		~AreaLighting();

		virtual RGBColor			// only overridden in the tracers SingleSphere and MultipleObjects
		trace_ray(const Ray& ray) const;

		virtual RGBColor
		trace_ray(const Ray ray, const int depth) const;

		virtual RGBColor
		trace_ray(const Ray ray, float& tmin, const int depth) const;

};

#endif
