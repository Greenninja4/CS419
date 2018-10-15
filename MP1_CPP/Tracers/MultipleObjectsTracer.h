#ifndef __MULTIPLE_OBJECTS_TRACER__
#define __MULTIPLE_OBJECTS_TRACER__

#include "Tracer.h"

class MultipleObjectsTracer : public Tracer{
    public:
        MultipleObjectsTracer(void);
        MultipleObjectsTracer(World* world_ptr);
        ~MultipleObjectsTracer(void);
        Vector3D traceRay(const Ray& ray) const;
};

#endif