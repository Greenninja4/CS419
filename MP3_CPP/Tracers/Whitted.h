#ifndef __WHITTED__
#define __WHITTED__

#include "Tracer.h"

class Whitted : public Tracer{
    public:
        Whitted(void);
        Whitted(World* world_ptr);
        virtual ~Whitted(void);
        virtual Vector3D trace_ray(const Ray& ray) const;
        virtual Vector3D trace_ray(const Ray ray, const int depth) const;
};

#endif
