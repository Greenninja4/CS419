#include "Plane.h"

#include "Constants.h"

#include <iostream>

Plane::Plane(void): 
    GeometricObject(), 
    p(0.0, 0.0, 0.0),
    n(0.0, 1.0, 0.0) {}
Plane::Plane(const Vector3D& p, const Vector3D& n): 
    GeometricObject(), 
    p(p),
    n(n)
    {this->n.normalize();}
Plane::Plane(const Plane& plane):
    GeometricObject(), 
    p(plane.p), 
    n(plane.n) {}
Plane& Plane::operator=(const Plane& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator=(rhs);
    p = rhs.p;
    n = rhs.n;
    return *this;
}
Plane::~Plane(void){}
Plane* Plane::clone(void) const{
    return (new Plane(*this));
}

// Hit function
bool Plane::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    double t = (p - ray.o) * n / (ray.d * n);
    if (t > KEPSILON){
        tmin = t;
        sr.normal = n;
        sr.localHitPoint = ray.o + t * ray.d;
        return true;
    } else{
        return false;
    }
}