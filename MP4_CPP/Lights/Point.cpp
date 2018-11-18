#include "Point.h"
#include "Constants.h"
#include "ShadeRec.h"
#include "World.h"
#include <iostream>

// Big 6
Point::Point(void):
    Light(), 
    ls(1.0), 
    color(WHITE), 
    location(0, 0, 0) {}
Point::Point(const double& ls, const Vector3D& color, const Vector3D& location):
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
bool Point::casts_shadow(void) const{
    return true;
};
bool Point::in_shadow(const Ray& ray, const ShadeRec& sr) const{
    double t;
    int num_objects = sr.world.objects.size();
    double d = location.distance(ray.o);
    for (int j = 0; j < num_objects; j++){
        if (sr.world.objects[j]->shadow_hit(ray, t) && (t < d)){
            return true;
        }
    }
    return false;
}