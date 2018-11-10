#include "Directional.h"
#include "Constants.h"

// Big 6
Directional::Directional(void): 
    Light(), 
    ls(1.0), 
    color(WHITE), 
    dir(0, 1, 0) {}
Directional::Directional(const double&  ls, const Vector3D& color, const Vector3D& dir): 
    Light(), 
    ls(ls), 
    color(color), 
    dir(dir) {
    this->dir.normalize();
}
Directional::Directional(const Directional& directional): 
    Light(directional), 
    ls(directional.ls), 
    color(directional.color), 
    dir(directional.dir) {}
Directional& Directional::operator= (const Directional& rhs){
    if (this == &rhs){
        return (*this);
    }
    Light::operator= (rhs);
    ls = rhs.ls;
    color = rhs.color;
    dir = rhs.dir;
    return *this;
}
Directional::~Directional(void){}
Light* Directional::clone(void) const{
    return (new Directional(*this));
}

// Functions
Vector3D Directional::get_direction(ShadeRec& sr){
    return dir;
}
Vector3D Directional::L(ShadeRec& sr){
    return ls * color;
}
bool Directional::casts_shadow(void) const {
    return true;
};
bool Directional::in_shadow(const Ray& ray, const ShadeRec& sr) const{
    return false;
}