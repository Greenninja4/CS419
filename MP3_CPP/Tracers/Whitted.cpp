#include "Whitted.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"
#include <iostream>

Whitted::Whitted(void):
    Tracer() {}
Whitted::Whitted(World* world_ptr):
    Tracer(world_ptr) {}
Whitted::~Whitted(void){}
Vector3D Whitted::trace_ray(const Ray& ray) const{
    ShadeRec sr(world_ptr->hit_objects(ray));
    if(sr.hit_an_object){
        sr.ray = ray;
        return sr.material_ptr->shade(sr);
    } else{
        return world_ptr->background_color;
    }
}
Vector3D Whitted::trace_ray(const Ray ray, const int depth) const{
    if (depth > world_ptr->vp.max_depth){
        return BLACK;
    }
    ShadeRec sr(world_ptr->hit_objects(ray));
    if(sr.hit_an_object){
        sr.depth = depth;
        sr.ray = ray;
        return sr.material_ptr->shade(sr);
    } else{
        return world_ptr->background_color;
    }
}