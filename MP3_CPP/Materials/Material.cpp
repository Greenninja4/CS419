#include "Material.h"
#include "Constants.h"

// Big 6
Material::Material(void){}
Material::Material(const Material& material){}
Material& Material::operator= (const Material& rhs){return (*this);}
Material::~Material(void){}

// Functions
Vector3D Material::shade(ShadeRec& sr){
    return BLACK;
}
Vector3D Material::area_light_shade(ShadeRec& sr){
    return BLACK;
}
Vector3D Material::get_Le(ShadeRec& sr) const{
    return BLACK;
}