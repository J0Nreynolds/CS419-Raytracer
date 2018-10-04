#ifndef __RAY_CAST__
#define __RAY_CAST__

#include "Tracer.h"

class RayCast : public Tracer {
	public:

		RayCast();

		RayCast(World* _world_ptr);

		virtual
		~RayCast();

		virtual RGBColor			// only overridden in the tracers SingleSphere and MultipleObjects
		trace_ray(const Ray& ray) const;

		virtual RGBColor
		trace_ray(const Ray ray, const int depth) const;

		virtual RGBColor
		trace_ray(const Ray ray, float& tmin, const int depth) const;

};

#endif
