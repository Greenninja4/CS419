#include "Light.h"
#include "Constants.h"

// Big 6
Light::Light(void){}
Light::Light(const Light& light){}
Light& Light::operator= (const Light& rhs){
    return (*this);
}
Light::~Light(void){}

// Functions
Vector3D Light::L(ShadeRec& sr){
    return BLACK;
}
bool Light::casts_shadow(void) const {
    return false;
};
bool Light::in_shadow(const Ray& ray, const ShadeRec& sr) const{
    return false;
}
float Light::G(const ShadeRec& sr) const{
    return 1;
}
float Light::pdf(const ShadeRec& sr) const{
    return 1;
}