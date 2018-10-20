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