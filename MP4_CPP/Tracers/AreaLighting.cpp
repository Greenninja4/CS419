#include "AreaLighting.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// Big 6
AreaLighting::AreaLighting(void):
    Tracer() {}
AreaLighting::AreaLighting(World* world_ptr):
    Tracer(world_ptr) {}
AreaLighting::~AreaLighting(){}

// Functions
Vector3D AreaLighting::trace_ray(const Ray& ray) const{
    ShadeRec sr(world_ptr->hit_objects(ray));
    if (sr.hit_an_object){
        sr.ray = ray;
        return sr.material_ptr->area_light_shade(sr);
    } else {
        return world_ptr->background_color;
    }
}

Vector3D AreaLighting::trace_ray(const Ray ray, const int depth) const{
    if (depth > world_ptr->vp.max_depth){
        return BLACK;
    }
    ShadeRec sr(world_ptr->hit_objects(ray));
    if (sr.hit_an_object){
        sr.depth = depth;
        sr.ray = ray;
        return sr.material_ptr->area_light_shade(sr);
    } else {
        return world_ptr->background_color;
    }
}