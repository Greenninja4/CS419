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
Vector3D RayCast::trace_ray(const Ray& ray) const{
    //cout << "RayCast: " << ray.o.x << ", " << ray.o.y << ", " << ray.o.z << endl;
    ShadeRec sr(world_ptr->hit_objects(ray));
    if (sr.hit_an_object){
        sr.ray = ray;
        return sr.material_ptr->shade(sr);
    } else {
        return world_ptr->background_color;
    }
}
Vector3D RayCast::trace_ray(const Ray ray, const int depth) const{
    ShadeRec sr(world_ptr->hit_objects(ray));
    if (sr.hit_an_object){
        sr.ray = ray;
        return sr.material_ptr->shade(sr);
    } else {
        return world_ptr->background_color;
    }
}