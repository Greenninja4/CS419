#include "ShadeRec.h"

#include "Constants.h"

ShadeRec::ShadeRec(World& world): 
    hitAnObject(false), 
    localHitPoint(), 
    normal(), 
    color(BLACK), 
    world(world) {}
ShadeRec::ShadeRec(const ShadeRec& sr): 
    hitAnObject(sr.hitAnObject),
    localHitPoint(sr.localHitPoint),
    normal(sr.normal),
    color(sr.color),
    world(sr.world) {}
ShadeRec& ShadeRec::operator=(const ShadeRec& rhs){
    hitAnObject = rhs.hitAnObject;
    localHitPoint = rhs.localHitPoint;
    normal = rhs.normal;
    color = rhs.color;
    return *this;
}
ShadeRec::~ShadeRec(void){}