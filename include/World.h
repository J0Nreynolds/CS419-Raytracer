#ifndef __WORLD__
#define __WORLD__

#include "Ray.h"
#include "RGBColor.h"
#include "ShadeRec.h"
#include "Sphere.h"

class World {
    public:
        ShadeRec hit_objects(Ray ray);
        RGBColor background_color;
        Sphere sphere;

};

#endif
