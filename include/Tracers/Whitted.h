#ifndef __WHITTED__
#define __WHITTED__

#include "Whitted.h"

class Whitted : public Tracer {
	public:

		Whitted();

		Whitted(World* _world_ptr);

		virtual
		~Whitted();

		virtual RGBColor
		trace_ray(const Ray& ray) const;

		virtual RGBColor
		trace_ray(const Ray ray, const int depth) const;

		virtual RGBColor
		trace_ray(const Ray ray, float& tmin, const int depth) const;

};

#endif
