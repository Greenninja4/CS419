#include "Point.h"
#include "Constants.h"
#include "ShadeRec.h"

// Big 6
Point::Point(void):
    Light(), 
    ls(1.0), 
    color(WHITE), 
    location(0, 0, 0) {}
Point::Point(const float& ls, const Vector3D& color, const Vector3D& location):
    Light(), 
    ls(ls), 
    color(color), 
    location(location) {}
Point::Point(const Point& point): 
    Light(point), 
    ls(point.ls), 
    color(point.color), 
    location(point.location) {}
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
Light* Point::clone(void) const{
    return (new Point(*this));
}

// Functions
Vector3D Point::get_direction(ShadeRec& sr){
    return ( (location - sr.hit_point).hat() );
}
Vector3D Point::L(ShadeRec& sr){
    return ls * color;
}