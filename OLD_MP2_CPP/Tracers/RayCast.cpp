#include "RayCast.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"
#include <iostream>

RayCast::RayCast(void):
    Tracer() {}
RayCast::RayCast(World* world_ptr): 
    Tracer(world_ptr) {}
RayCast::~RayCast(void){}
Vector3D RayCast::traceRay(const Ray& ray) const{
    //cout << "RayCast: " << ray.o.x << ", " << ray.o.y << ", " << ray.o.z << endl;
    ShadeRec sr(world_ptr->hitObjects(ray));
    if (sr.hitAnObject){
        sr.ray = ray;
        return sr.material_ptr->shade(sr);
    } else {
        return world_ptr->backgroundColor;
    }
}
Vector3D RayCast::traceRay(const Ray ray, const int depth) const{
    ShadeRec sr(world_ptr->hitObjects(ray));
    if (sr.hitAnObject){
        sr.ray = ray;
        return sr.material_ptr->shade(sr);
    } else {
        return world_ptr->backgroundColor;
    }
}