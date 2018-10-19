#include "Ray.h"

// Big 6
Ray::Ray(void): 
    o(0.0, 0.0, 0.0), 
    d(0.0, 0.0, 1.0) {}
Ray::Ray(const Vector3D& o, const Vector3D& d): 
    o(o), 
    d(d) {}
Ray::Ray(const Ray& ray): 
    o(ray.o), 
    d(ray.d) {}
Ray& Ray::operator= (const Ray& rhs){
    o = rhs.o;
    d = rhs.d;
    return *this;
}
Ray::~Ray(){}

// Getters & Setters
void Ray::set_o(const Vector3D& o){
    this->o = o;
}
void Ray::set_d(const Vector3D& d){
    this->d = d;
}
Vector3D Ray::get_o(void) const{
    return o;
}
Vector3D Ray::get_d(void) const{
    return d;
}