#include "Constants.h"
#include "BRDF.h"

BRDF::BRDF(void){}
BRDF::BRDF(const BRDF& brdf){}
BRDF& BRDF::operator= (const BRDF& rhs){
    return *this;
}
BRDF::~BRDF(void){}

Vector3D BRDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return BLACK;
}
Vector3D BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const {
    return BLACK;
}
Vector3D BRDF::rho(const ShadeRec& sr, Vector3D& wo) const {
    return BLACK;
}