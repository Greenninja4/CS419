#include "Sphere.h"

#include "Constants.h"
#include <cmath>

Sphere::Sphere(void): 
    GeometricObject(), 
    center(0.0, 0.0, 0.0),
    radius(1.0) {}
Sphere::Sphere(Vector3D center, double radius, const Vector3D color): 
    GeometricObject(), 
    center(center),
    radius(radius)
    {this->color = color;}
Sphere::Sphere(const Sphere& sphere){
    center = sphere.center;
    radius = sphere.radius;
    color = sphere.color;
}
Sphere& Sphere::operator=(const Sphere& rhs){
    center = rhs.center;
    radius = rhs.radius;
    color = rhs.color;
    return *this;
}
Sphere::~Sphere (void){}

// Hit function
bool Sphere::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    double t;
    Vector3D temp = ray.o - center;
    double a = ray.d * ray.d;
    double b = 2.0 * temp * ray.d;
    double c = temp * temp - radius * radius;
    double disc = b * b - 4.0 * a * c;

    if (disc < 0.0){
        return false;
    } else {
        double e = sqrt(disc);
        double denom = 2.0 * a;

        // Smaller root
        t = (-b - e) / denom;
        if (t > KEPSILON){
            tmin = t;
            sr.normal = (temp + t * ray.d) / radius;
            sr.localHitPoint = ray.o - t * ray.d;
            return true;
        }

        // Larger root
        t = (-b - e) / denom;
        if (t > KEPSILON){
            tmin = t;
            sr.normal = (temp + t * ray.d) / radius;
            sr.localHitPoint = ray.o - t * ray.d;
            return true;
        }
    }
    return false;
}