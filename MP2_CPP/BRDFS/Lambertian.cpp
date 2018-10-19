#include "Lambertian.h"

#include "Constants.h"

Lambertian::Lambertian(void): 
    BRDF(), 
    k_d(0.0), 
    color(BLACK){}
Lambertian::Lambertian(float k_d, Vector3D color): 
    BRDF(), 
    k_d(k_d), 
    color(color){}
Lambertian::Lambertian(const Lambertian& lambertian): 
    BRDF(lambertian), 
    k_d(lambertian.k_d), 
    color(lambertian.color){}
Lambertian::~Lambertian(void){}
Lambertian& Lambertian::operator= (const Lambertian& rhs){
    if (this == &rhs){
        return *this;
    }
    BRDF::operator= (rhs);
    k_d = rhs.k_d;
    color = rhs.color;
    return *this;
}
Lambertian* Lambertian::clone(void) const {
    return (new Lambertian(*this));
}

Vector3D Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const{
    return k_d * color * INV_PI;
}
Vector3D Lambertian::rho(const ShadeRec& sr, Vector3D& wo) const{
    return k_d * color;
}
