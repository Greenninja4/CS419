#include "Directional.h"
#include "Constants.h"

Directional::Directional(void): 
    Light(), 
    ls(1.0), 
    color(WHITE), 
    dir(0, 1, 0) {}
Directional::Directional(float ls, Vector3D color, Vector3D dir): 
    Light(), 
    ls(ls), 
    color(color), 
    dir(dir) {
    dir.normalize();
}
Directional::Directional(const Directional& directional): 
    Light(directional), 
    ls(directional.ls), 
    color(directional.color), 
    dir(directional.dir) {}
Light* Directional::clone(void) const{
    return (new Directional(*this));
}
Directional& Directional::operator=(const Directional& rhs){
    if (this == &rhs){
        return (*this);
    }
    Light::operator=(rhs);
    ls = rhs.ls;
    color = rhs.color;
    dir = rhs.dir;
    
    return (*this);
}
Directional::~Directional(void){}

Vector3D Directional::get_direction(ShadeRec& sr){
    return dir;
}
Vector3D Directional::L(ShadeRec& sr){
    return ls * color;
}