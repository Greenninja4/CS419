#include "Specular.h"

#include "Constants.h"

Specular::Specular(void): 
    BRDF(), 
    k_r(0.0), 
    color(WHITE){}
Specular::Specular(float k_r, Vector3D color): 
    BRDF(), 
    k_r(k_r), 
    color(color){}
Specular::Specular(const Specular& specular): 
    BRDF(specular), 
    k_r(specular.k_r), 
    color(specular.color){}
Specular::~Specular(void){}
Specular& Specular::operator=(const Specular& rhs){
    if (this == &rhs){
        return *this;
    }
    BRDF::operator= (rhs);
    k_r = rhs.k_r;
    color = rhs.color;
    return *this;
}
Specular* Specular::clone(void) const {
    return (new Specular(*this));
}

Vector3D Specular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const{
    return BLACK;
}
Vector3D Specular::rho(const ShadeRec& sr, Vector3D& wo) const{
    return BLACK;
}
