#include "GeometricObject.h"

GeometricObject::GeometricObject(void): color(0.0, 0.0, 0.0){}
GeometricObject::GeometricObject(const GeometricObject& object){
    color = object.color;
}
GeometricObject& GeometricObject::operator=(const GeometricObject& rhs){
    color = rhs.color;
    return *this;
}
GeometricObject::~GeometricObject(void){}