#ifndef __AREA_LIGHTING__
#define __AREA_LIGHTING__

#include "Tracer.h"

class AreaLighting : public Tracer{
    public:
        AreaLighting(void);
        AreaLighting(World* world_ptr);
        virtual ~AreaLighting();
        virtual Vector3D trace_ray(const Ray& ray) const;
        virtual Vector3D trace_ray(const Ray ray, const int depth) const;
};

#endif