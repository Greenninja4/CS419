#include "ShadeRec.h"

#include "Constants.h"

ShadeRec::ShadeRec(World& world): 
    hitAnObject(false), 
    material_ptr(NULL), 
    hitPoint(), 
    localHitPoint(), 
    normal(), 
    ray(), 
    depth(0), 
    t(0.0), 
    world(world) {}
ShadeRec::ShadeRec(const ShadeRec& sr): 
    hitAnObject(sr.hitAnObject), 
    material_ptr(sr.material_ptr), 
    hitPoint(sr.hitPoint), 
    localHitPoint(sr.localHitPoint), 
    normal(sr.normal), 
    ray(sr.ray), 
    depth(sr.depth), 
    t(sr.t), 
    world(sr.world) {}
ShadeRec::~ShadeRec(void){}
