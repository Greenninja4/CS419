#ifndef __TRACER__
#define __TRACER__

class World;
#include "Vector3D.h"
#include "Ray.h"

class Tracer{
    public:
        // World
        World* world_ptr;

        Tracer(void);
        Tracer(World* world_ptr);
        virtual ~Tracer(void);
        virtual Vector3D traceRay(const Ray& ray) const;
        Vector3D traceRay(const Ray ray, const int depth) const;
};

#endif