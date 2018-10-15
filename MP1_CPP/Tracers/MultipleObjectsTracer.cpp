#include "MultipleObjectsTracer.h"

#include "World.h"

#include <iostream>

MultipleObjectsTracer::MultipleObjectsTracer(void): Tracer(){}
MultipleObjectsTracer::MultipleObjectsTracer(World* world_ptr): Tracer(world_ptr) {}
MultipleObjectsTracer::~MultipleObjectsTracer(void){}

Vector3D MultipleObjectsTracer::traceRay(const Ray& ray) const{
    ShadeRec sr(world_ptr->hitBareBonesObjects(ray));
    if (sr.hitAnObject){
        return sr.color;
    } else {
        return world_ptr->backgroundColor;
    }
}