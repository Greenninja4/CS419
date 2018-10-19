#ifndef __RAY_CAST__
#define __RAY_CAST__

#include "Tracer.h"

class RayCast : public Tracer {
    public:
        RayCast(void);
        RayCast(World* world_ptr);
        virtual ~RayCast(void);
        virtual Vector3D traceRay(const Ray& ray) const;
        virtual Vector3D traceRay(const Ray ray, const int depth) const;
};

#endif