#include "GlossySpecular.h"
#include "Constants.h"
#include <math.h>

// Big 6
GlossySpecular::GlossySpecular(void): 
    BRDF(), 
    k_s(0.0), 
    color(WHITE), 
    exp(1.0) {}
GlossySpecular::GlossySpecular(const double& k_s, const Vector3D& color, const double& exp): 
    BRDF(), 
    k_s(k_s), 
    color(color), 
    exp(exp) {}
GlossySpecular::GlossySpecular(const GlossySpecular& glossy_specular): 
    BRDF(glossy_specular), 
    k_s(glossy_specular.k_s), 
    color(glossy_specular.color), 
    exp(glossy_specular.exp) {}
GlossySpecular::~GlossySpecular(void){}
GlossySpecular& GlossySpecular::operator= (const GlossySpecular& rhs){
    if (this == &rhs){
        return *this;
    }
    BRDF::operator= (rhs);
    k_s = rhs.k_s;
    color = rhs.color;
    this->exp = rhs.exp;
    return *this;
}
GlossySpecular* GlossySpecular::clone(void) const {
    return (new GlossySpecular(*this));
}

// Functions
Vector3D GlossySpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const{
    Vector3D L = BLACK;
    double ndotwi = sr.normal * wi;
    Vector3D r(-wi + 2.0 * sr.normal * ndotwi);
    double rdotwo = r * wo;
    if (rdotwo > 0.0){
        L = k_s * pow(rdotwo, this->exp);
    }
    return L;
}
Vector3D GlossySpecular::rho(const ShadeRec& sr, Vector3D& wo) const{
    return BLACK;
}
