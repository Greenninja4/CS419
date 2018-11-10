#include "Material.h"
#include "Constants.h"

Material::Material(void){}
Material::Material(const Material& material){}
Material& Material::operator= (const Material& rhs){return (*this);}
Material::~Material(void){}
Vector3D Material::shade(ShadeRec& sr){
    return BLACK;
}