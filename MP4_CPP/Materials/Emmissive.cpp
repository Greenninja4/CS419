#include "Emmissive.h"

// Big 6
Emmissive::Emmissive(void):
    Material(),
    ls(1),
    ce(WHITE) {}
Emmissive::Emmissive(float ls, const Vector3D& ce):
    Material(),
    ls(ls),
    ce(ce) {}
Emmissive::Emmissive(const Emmissive& emmissive):
    Material(emmissive),
    ls(emmissive.ls),
    ce(emmissive.ce) {}
Emmissive& Emmissive::operator= (const Emmissive& rhs){
    if (this == &rhs){
        return *this;
    }
    Material::operator=(rhs);
    ls = rhs.ls;
    ce = rhs.ce;
    return *this;
}
Emmissive::~Emmissive(void){}
Material* Emmissive::clone(void) const{
    return (new Emmissive(*this));
}

// Functions
Vector3D Emmissive::shade(ShadeRec& sr){
    if (sr.normal * sr.ray.d < 0.0){
        return ls * ce;
    } else {
        return BLACK;
    }
}
Vector3D Emmissive::area_light_shade(ShadeRec& sr){
    if (sr.normal * sr.ray.d < 0.0){
        return ls * ce;
    } else {
        return BLACK;
    }
}
Vector3D Emmissive::get_Le(ShadeRec& sr) const{
    if (sr.normal * sr.ray.d < 0.0){
        return ls * ce;
    } else {
        return BLACK;
    }
}