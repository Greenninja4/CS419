#include "Tracer.h"

// Big 6
Tracer::Tracer(void): 
    world_ptr(NULL) {}
Tracer::Tracer(World* world_ptr): 
    world_ptr(world_ptr) {}
Tracer::~Tracer(void){}

// Functions
Vector3D Tracer::trace_ray(const Ray& ray) const{
    return BLACK;
}
Vector3D Tracer::trace_ray(const Ray ray, const int depth) const{
    return BLACK;
}