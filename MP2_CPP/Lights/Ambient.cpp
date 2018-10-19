#include "Ambient.h"
#include "Constants.h"

Ambient::Ambient(void): 
    Light(), 
    ls(1.0), 
    color(WHITE) {}
Ambient::Ambient(float ls, Vector3D color): 
    Light(), 
    ls(ls), 
    color(color) {}
Ambient::Ambient(const Ambient& ambient): 
    Light(ambient), 
    ls(ambient.ls), 
    color(ambient.color) {}
Light* Ambient::clone(void) const{ 
    return (new Ambient(*this));
}
Ambient& Ambient::operator= (const Ambient& rhs){
    if (this == &rhs){
        return *this;
    }
    Light::operator= (rhs);
    ls = rhs.ls;
    color = rhs.color;
    return *this;
}
Ambient::~Ambient(void){}

Vector3D Ambient::get_direction(ShadeRec& sr){
    return (Vector3D(0, 0, 0));
}
Vector3D Ambient::L(ShadeRec& sr){
    return ls * color;
}