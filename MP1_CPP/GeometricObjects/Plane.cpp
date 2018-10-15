#include "Plane.h"

#include "Constants.h"

#include <iostream>

Plane::Plane(void): 
    GeometricObject(), 
    p(0.0, 0.0, 0.0),
    n(0.0, 1.0, 0.0) {}
Plane::Plane(const Vector3D& p, const Vector3D& n, const Vector3D& color): 
    GeometricObject(), 
    p(p),
    n(n)
    {this->n.normalize();
    this->color = color;}
Plane::Plane(const Plane& plane){
    p = plane.p;
    n = plane.n;
    color = plane.color;
}
Plane& Plane::operator=(const Plane& rhs){
    p = rhs.p;
    n = rhs.n;
    color = rhs.color;
    return *this;
}
Plane::~Plane (void){}

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