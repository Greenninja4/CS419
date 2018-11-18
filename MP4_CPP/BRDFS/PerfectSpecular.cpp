#include "PerfectSpecular.h"

#include "Constants.h"

PerfectSpecular::PerfectSpecular(void): 
    BRDF(), 
    k_r(0.0), 
    color(WHITE){}
PerfectSpecular::PerfectSpecular(float k_r, Vector3D color): 
    BRDF(), 
    k_r(k_r), 
    color(color){}
PerfectSpecular::PerfectSpecular(const PerfectSpecular& specular): 
    BRDF(specular), 
    k_r(specular.k_r), 
    color(specular.color){}
PerfectSpecular::~PerfectSpecular(void){}
PerfectSpecular& PerfectSpecular::operator= (const PerfectSpecular& rhs){
    if (this == &rhs){
        return *this;
    }
    BRDF::operator= (rhs);
    k_r = rhs.k_r;
    color = rhs.color;
    return *this;
}
PerfectSpecular* PerfectSpecular::clone(void) const {
    return (new PerfectSpecular(*this));
}

Vector3D PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const{
    float ndotwo = sr.normal * wo;
    wi = -wo + 2.0 * sr.normal * ndotwo;
    return (k_r * color / (sr.normal * wi));
}
Vector3D PerfectSpecular::rho(const ShadeRec& sr, Vector3D& wo) const{
    return BLACK;
}
