#include "ShadeRec.h"
#include "Constants.h"
#include <iostream>
using namespace std;

// Big 6
ShadeRec::ShadeRec(const World& world): 
    hit_an_object(false), 
    material_ptr(NULL), 
    hit_point(), 
    local_hit_point(), 
    normal(), 
    ray(), 
    depth(0), 
    t(0.0), 
    u(0.0), 
    v(0.0), 
    world(world) {}
ShadeRec::ShadeRec(const ShadeRec& sr): 
    hit_an_object(sr.hit_an_object), 
    material_ptr(sr.material_ptr), 
    hit_point(sr.hit_point), 
    local_hit_point(sr.local_hit_point), 
    normal(sr.normal), 
    ray(sr.ray), 
    depth(sr.depth), 
    t(sr.t), 
    u(sr.u), 
    v(sr.v), 
    world(sr.world) {}
ShadeRec& ShadeRec::operator= (const ShadeRec& rhs){
    if (this == &rhs){
        return *this;
    }
    hit_an_object = rhs.hit_an_object;
    material_ptr = rhs.material_ptr;
    hit_point = rhs.hit_point;
    local_hit_point = rhs.local_hit_point;
    normal = rhs.normal;
    ray = rhs.ray;
    depth = rhs.depth;
    t = rhs.t;
    u = rhs.u;
    v = rhs.v;
    return *this;
}
ShadeRec::~ShadeRec(void){}
