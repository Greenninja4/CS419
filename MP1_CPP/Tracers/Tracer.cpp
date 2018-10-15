#include "Tracer.h"

#include "World.h"
#include "Constants.h"

Tracer::Tracer(void): world_ptr(NULL){}
Tracer::Tracer(World* world_ptr): world_ptr(world_ptr) {}
Vector3D Tracer::traceRay(const Ray& ray) const{
    return BLACK;
}