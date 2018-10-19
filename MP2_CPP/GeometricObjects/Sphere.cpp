#include "Sphere.h"
#include "Constants.h"
#include <math.h>

// Big 6
Sphere::Sphere(void): 
    GeometricObject(), 
    center(0.0, 0.0, 0.0), 
    radius(1.0) {}
Sphere::Sphere(const Vector3D& center, const double& radius): 
    GeometricObject(), 
    center(center),
    radius(radius) {}
Sphere::Sphere(const Sphere& sphere): 
    GeometricObject(sphere), 
    center(sphere.center), 
    radius(sphere.radius) {}
Sphere& Sphere::operator= (const Sphere& rhs){
    if (this == &rhs){
        return *this;
    }
    GeometricObject::operator= (rhs);
    center = rhs.center;
    radius = rhs.radius;
    return *this;
}
Sphere::~Sphere (void){}
Sphere* Sphere::clone(void) const{
    return (new Sphere(*this));
}

// Getters & Setters
void Sphere::set_center(const Vector3D& center){
    this->center = center;
}
void Sphere::set_radius(const double& radius){
    this->radius = radius;
}

// Functions
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
            sr.local_hit_point = ray.o - t * ray.d;
            return true;
        }

        // Larger root
        t = (-b + e) / denom;
        if (t > KEPSILON){
            tmin = t;
            sr.normal = (temp + t * ray.d) / radius;
            sr.local_hit_point = ray.o - t * ray.d;
            return true;
        }
    }
    return false;
}
