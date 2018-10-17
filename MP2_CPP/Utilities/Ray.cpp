#include "Ray.h"

Ray::Ray(void): o(0.0, 0.0, 0.0), d(0.0, 0.0, 1.0) {}
Ray::Ray(const Vector3D& o, const Vector3D& d): o(o), d(d) {}
Ray::Ray(const Ray& ray): o(ray.o), d(ray.d) {}
Ray& Ray::operator=(const Ray& rhs){
    o = rhs.o;
    d = rhs.d;
    return *this;
}
Ray::~Ray(){}