#include "Point.h"
#include "Constants.h"
#include "ShadeRec.h"

Point::Point(void):
    Light(), 
    ls(1.0), 
    color(WHITE), 
    location(0, 0, 0) {}
Point::Point(float ls, Vector3D color, Vector3D location):
    Light(), 
    ls(ls), 
    color(color), 
    location(location) {}
Point::Point(const Point& point): 
    Light(point), 
    ls(point.ls), 
    color(point.color), 
    location(point.location) {}
Light* Point::clone(void) const{
    return (new Point(*this));
}
Point& Point::operator= (const Point& rhs){
    if (this == &rhs){
        return *this;
    }
    Light::operator= (rhs);
    ls = rhs.ls;
    color = rhs.color;
    location = rhs.location;
    return *this;
}

Point::~Point(void){}
Vector3D Point::get_direction(ShadeRec& sr){
    return ( (location - sr.hit_point).hat() );
}
Vector3D Point::L(ShadeRec& sr){
    return ls * color;
}