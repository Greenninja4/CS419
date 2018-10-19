#include "Tracer.h"

#include "World.h"
#include "Constants.h"

Tracer::Tracer(void): world_ptr(NULL){}
Tracer::Tracer(World* world_ptr): world_ptr(world_ptr) {}
Tracer::~Tracer(void){
    if (world_ptr){
        world_ptr = NULL;
    }
}

Vector3D Tracer::traceRay(const Ray& ray) const{
    return BLACK;
}
Vector3D Tracer::traceRay(const Ray ray, const int depth) const{
    return BLACK;
}