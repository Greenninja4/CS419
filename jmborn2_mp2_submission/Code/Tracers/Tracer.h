#ifndef __TRACER__
#define __TRACER__

#include "Constants.h"
#include "Ray.h"
#include "Vector3D.h"

class World; // Forward declaration because World has a Tracer*

class Tracer{
    public:
        // Variables
        World* world_ptr; // World

        // Big 6
        Tracer(void);
        Tracer(World* world_ptr);
        virtual ~Tracer(void);

        // Functions
        virtual Vector3D trace_ray(const Ray& ray) const;
        virtual Vector3D trace_ray(const Ray ray, const int depth) const;
};

#endif